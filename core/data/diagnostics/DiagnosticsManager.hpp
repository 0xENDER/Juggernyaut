/**
 * @brief
 * Grouping, converting, and flushing diagnostics by file or component.
**/

#pragma once

#include "common/headers.hpp"

// Diagnostics
#include "Diagnostic.hpp"

// Driver (ONLY THE HOOKS!)
#include "../../driver/Hooks.hpp"

using FrontEndHooks = Driver::Hooks;
namespace Data::Diagnostics {
    // Controls how incoming diagnostics are retained and flushed across compilation stages
    enum class ManagerMode : uint8_t {
        Default,   // Groups diagnostics per file within each stage, flushing and clearing at each stage boundary
        Eager,     // Retains cumulative per-file diagnostics across intermediate stages and emits empty flushes (for LSP)
        Funnelled  // Immediately flushes each received diagnostic with zero retention while keeping stats updated (for CLI debugging)
    };

    // NOTE: Final stage flusing logic can change as more compilation phases are added!
    // WARNING: Your DiagnosticsManager should be accessed by one thread at a time!
    template <typename TargetDiag = Diagnostic>
    class DiagnosticsManager {
        public:
            using Mode = ManagerMode;
            using ConverterFn = std::function<TargetDiag(const Diagnostic&)>; // This is done to prevent data duplication in memory as much as possible when dealing with different frameworks!
            using FileFlushCallback = std::function<void(const std::string &, const std::vector<TargetDiag> &)>;
            using ComponentFlushCallback = std::function<void(const TargetDiag &)>;

            struct ManagerCallbacks {
                FileFlushCallback onFileFlush = nullptr;
                ComponentFlushCallback onComponentFlush = nullptr;
            };

            struct Stats {
                uint32_t filesCompiled = 0; // Triggered `onContextStart`
                uint32_t filesSkipped = 0;  // Up-to-date (`onContextSkip`)
                uint32_t filesDetached = 0; // Pruned (`onContextDetached`)

                // Reported diagnostics
                uint32_t errors = 0;
                uint32_t warnings = 0;
                uint32_t hints = 0;
                uint32_t info = 0;

                [[nodiscard]] constexpr uint32_t filesVisited() const noexcept {
                    return this->filesCompiled + this->filesSkipped;
                }
                [[nodiscard]] constexpr bool hasErrors() const noexcept {
                    return this->errors > 0u;
                }
            };
        private:
            struct FileDiagState {
                std::vector<TargetDiag> diagnostics;
                bool isDetached = false;
                bool needsFlush = true; // Reset diagnostics by default!
            };

            std::mutex hookMtx;
            std::unordered_map<std::string, FileDiagState> fileDiagnostics;
            std::vector<TargetDiag> componentDiagnostics; // diag.file == nullptr
            const ConverterFn converter;
            const ManagerCallbacks callbacks;
            const Mode mode = Mode::Default;
            Stats stats{};

            // Internal (`hookMtx` must be held by caller)
            void recordDiagStats(Severity severity) noexcept {
                switch (severity) {
                    case Severity::Error: {
                        ++(this->stats.errors);
                        break;
                    }
                    case Severity::Warning: {
                        ++(this->stats.warnings);
                        break;
                    }
                    case Severity::Hint: {
                        ++(this->stats.hints);
                        break;
                    }
                    default:
                    case Severity::Info: {
                        ++(this->stats.info);
                        break;
                    }
                }
            }

            // Record a diagnostic (copies URI string immediately while inside the event)
            void push(const Diagnostic &diag) {
                if (this->converter == nullptr) {
                    std::lock_guard<std::mutex> lock(this->hookMtx);
                    this->recordDiagStats(diag.severity);
                    return;
                }
                TargetDiag convertedDiag = this->converter(diag);

                std::lock_guard<std::mutex> lock(this->hookMtx);
                this->recordDiagStats(diag.severity);

                // Funnelled mode: flush immediately with zero retention
                if (this->mode == Mode::Funnelled) {
                    if (diag.file != nullptr) {
                        if (this->callbacks.onFileFlush != nullptr) {
                            std::vector<TargetDiag> singleDiag;
                            singleDiag.reserve(1);
                            singleDiag.push_back(std::move(convertedDiag));
                            this->callbacks.onFileFlush(diag.file->canonical, singleDiag);
                        }
                    } else if (this->callbacks.onComponentFlush != nullptr) {
                        this->callbacks.onComponentFlush(convertedDiag);
                    }
                    return;
                }

                if (diag.file != nullptr) {
                    FileDiagState &state = this->fileDiagnostics[diag.file->canonical];

                    state.diagnostics.push_back(std::move(convertedDiag));
                    state.needsFlush = true;
                } else if (this->mode == Mode::Default) {
                    if (this->callbacks.onComponentFlush != nullptr) {
                        this->callbacks.onComponentFlush(convertedDiag);
                    }
                } else {
                    this->componentDiagnostics.push_back(std::move(convertedDiag));
                }
            }

            // Flush a single file by canonical URI:
            // - clearAfter == false (for Intermediate stages in Eager mode)
            // - clearAfter == true (for Final stage, or every stage in Default mode)
            // - sendEmpty == true (for Eager/LSP mode to clear stale editor squiggles)
            void flushFile(const std::string &uri, bool clearAfter = false, bool sendEmpty = true) {
                std::lock_guard<std::mutex> lock(this->hookMtx);

                auto it = this->fileDiagnostics.find(uri);
                if (it != this->fileDiagnostics.end()) {
                    if (this->callbacks.onFileFlush != nullptr && it->second.needsFlush) {
                        if (sendEmpty || !it->second.diagnostics.empty()) {
                            this->callbacks.onFileFlush(uri, it->second.diagnostics);
                        }
                        it->second.needsFlush = false;
                    }

                    if (clearAfter) {
                        this->fileDiagnostics.erase(it); // Free memory!
                    }
                } else if (sendEmpty && this->callbacks.onFileFlush != nullptr) {
                    // The diagnostics list isn't present!

                    // File finished its final stage with 0 diagnostics: send empty `[]` to clear any old squiggles in the editor!
                    static const std::vector<TargetDiag> emptyList;
                    this->callbacks.onFileFlush(uri, emptyList);
                }
            }
            void emptyFlush(const std::string &uri) {
                if (this->callbacks.onFileFlush == nullptr) {
                    return;
                }
                std::lock_guard<std::mutex> lock(this->hookMtx);

                static const std::vector<TargetDiag> emptyList;
                this->callbacks.onFileFlush(uri, emptyList);
            }

            // Internal (Diagnostic dispatch logic)
            void setupDiagnosticDispatch(FrontEndHooks &frontEnd) {
                // Handle incoming Diagnostics
                frontEnd.driver.onDiagnostic = [this](const Diagnostic &diag) {
                    this->push(diag);
                };

                // Funnelled mode: immediate flush in `push()`, stats tracking only, no retention
                if (this->mode == Mode::Funnelled) {
                    frontEnd.unitSources.onContextStart << [this](const File *file) {
                        if (file == nullptr) {
                            return;
                        }
                        std::lock_guard<std::mutex> lock(this->hookMtx);
                        ++(this->stats.filesCompiled);
                    };
                    frontEnd.unitSources.onContextSkip << [this](const File *file) {
                        if (file == nullptr) {
                            return;
                        }
                        std::lock_guard<std::mutex> lock(this->hookMtx);
                        ++(this->stats.filesSkipped);
                    };
                    frontEnd.unitSources.onContextDetached << [this](const File *file) {
                        if (file == nullptr) {
                            return;
                        }
                        std::lock_guard<std::mutex> lock(this->hookMtx);
                        ++(this->stats.filesDetached);
                    };
                    return;
                }

                // Default mode: group diagnostics per file within each stage, then flush and clear on stage end
                if (this->mode == Mode::Default) {
                    FileEvent flushStageAndClear = [this](const Data::Host::File *file) {
                        if (file != nullptr) {
                            this->flushFile(file->canonical, true, false);
                        }
                    };

                    // [jug.toml]
                    frontEnd.driver.onEnvironmentLoad << flushStageAndClear;
                    frontEnd.driver.onEnvironmentChange << flushStageAndClear;

                    // [*.jug]
                    frontEnd.unitSources.onContextStart << [this](const File *file) {
                        if (file == nullptr) {
                            return;
                        }
                        std::lock_guard<std::mutex> lock(this->hookMtx);
                        this->fileDiagnostics.erase(file->canonical);
                        ++(this->stats.filesCompiled);
                    };
                    frontEnd.unitSources.onParserContextEnd << flushStageAndClear;
                    frontEnd.unitSources.onContextEnd << flushStageAndClear;
                    frontEnd.unitSources.onContextSkip << [this](const File *file) {
                        if (file == nullptr) {
                            return;
                        }
                        std::lock_guard<std::mutex> lock(this->hookMtx);
                        ++(this->stats.filesSkipped);
                    };
                    frontEnd.unitSources.onContextDetached << [this](const File *file) {
                        if (file == nullptr) {
                            return;
                        }
                        std::lock_guard<std::mutex> lock(this->hookMtx);
                        this->fileDiagnostics.erase(file->canonical);
                        ++(this->stats.filesDetached);
                    };
                    frontEnd.driver.onRunFinished << [this]() {
                        this->manualFlushAll(true);
                    };
                    return;
                }

                // Eager mode: cumulative per-file retention across intermediate stages + empty flushes (for LSP)
                FileEvent flushAndKeep = [this](const Data::Host::File *file) {
                    if (file != nullptr) {
                        this->flushFile(file->canonical, false, true);
                    }
                };
                FileEvent flushAndDiscard = [this](const Data::Host::File *file) {
                    if (file != nullptr) {
                        this->flushFile(file->canonical, true, true);
                    }
                };

                // Handle Diagnostics Retention logic

                // [jug.toml]
                frontEnd.driver.onEnvironmentLoad << flushAndDiscard;
                frontEnd.driver.onEnvironmentChange << flushAndDiscard;

                // [*.jug]
                frontEnd.unitSources.onContextStart << [this](const File *file) {
                    if (file == nullptr) {
                        return;
                    }
                    std::lock_guard<std::mutex> lock(this->hookMtx);

                    // Update state and release previous vector capacity
                    FileDiagState &state = this->fileDiagnostics[file->canonical];
                    state.isDetached = false;
                    std::vector<TargetDiag>{}.swap(state.diagnostics); // Free up memory!
                    state.needsFlush = true;

                    // Tracking
                    ++(this->stats.filesCompiled);
                };
                frontEnd.unitSources.onParserContextEnd << flushAndKeep;
                frontEnd.unitSources.onContextEnd << flushAndDiscard;
                frontEnd.unitSources.onContextSkip << [this](const File *file) {
                    if (file == nullptr) {
                        return;
                    }
                    // Refresh detached files (in case no edits were made!)
                    bool shouldFlush = false;
                    {
                        std::lock_guard<std::mutex> lock(this->hookMtx);
                        if (auto it = this->fileDiagnostics.find(file->canonical);
                            it != this->fileDiagnostics.end() && it->second.isDetached) {
                            it->second.isDetached = false;
                            it->second.needsFlush = true;
                            shouldFlush = true;
                        }
                        // Tracking
                        ++(this->stats.filesSkipped);
                    }
                    if (shouldFlush) {
                        this->flushFile(file->canonical, false, true);
                    }
                };
                frontEnd.unitSources.onContextDetached << [this](const File *file) {
                    if (file == nullptr) {
                        return;
                    }
                    {
                        std::lock_guard<std::mutex> lock(this->hookMtx);
                        this->fileDiagnostics[file->canonical].isDetached = true;
                        // Tracking
                        ++(this->stats.filesDetached);
                    }
                    this->emptyFlush(file->canonical);
                };
                frontEnd.driver.onRunFinished << [this]() {
                    this->flushComponentDiagnostics(true);
                };
            }

        public:
            // Constructor: overtakes `diagnosticHandle` and converts incoming diagnostics
            DiagnosticsManager(
                FrontEndHooks &hooks,
                ConverterFn convertFn,
                ManagerCallbacks managerCallbacks,
                Mode managerMode = Mode::Default
            ) : converter(std::move(convertFn)), callbacks(std::move(managerCallbacks)), mode(managerMode) {
                setupDiagnosticDispatch(hooks);
            }

            // Overload when TargetDiag == Diagnostics::Diagnostic (no converter needed)
            DiagnosticsManager(
                FrontEndHooks &hooks,
                ManagerCallbacks managerCallbacks,
                Mode managerMode = Mode::Default
            ) requires std::same_as<TargetDiag, Diagnostic>
                : DiagnosticsManager(
                    hooks,
                    [](const Diagnostic &d) -> Diagnostic { return d; },
                    std::move(managerCallbacks),
                    managerMode
                ) {}
            ~DiagnosticsManager() = default;

            // Non-copyable (since the bound lambda captures `this`)
            DiagnosticsManager(const DiagnosticsManager&) = delete;
            DiagnosticsManager& operator=(const DiagnosticsManager&) = delete;
            DiagnosticsManager(DiagnosticsManager&&) = delete;
            DiagnosticsManager& operator=(DiagnosticsManager&&) = delete;

            [[nodiscard]] constexpr Mode getMode() const noexcept {
                return this->mode;
            }

            // Prepare for a new run: in Eager mode, clear vectors, but KEEP keys that had errors on the last run
            // so if they have 0 errors on this run, `flush()` sends an empty `[]` to clear the editor!
            void clearAllDiagnostics() {
                std::lock_guard<std::mutex> lock(this->hookMtx);

                if (this->mode == Mode::Eager) {
                    for (auto &[uri, state] : this->fileDiagnostics) {
                        state.diagnostics.clear();
                    }
                } else {
                    this->fileDiagnostics.clear();
                }
                this->componentDiagnostics.clear();
            }

            // Flush all queued component-wide diagnostics (diag.file == nullptr)
            void flushComponentDiagnostics(bool clearAfter = true) {
                std::vector<TargetDiag> toFlush;
                {
                    std::lock_guard<std::mutex> lock(this->hookMtx);
                    if (this->componentDiagnostics.empty()) {
                        return;
                    }
                    // Free up unused components
                    if (this->callbacks.onComponentFlush == nullptr) {
                        if (clearAfter) {
                            std::vector<TargetDiag>{}.swap(this->componentDiagnostics);
                        }
                        return;
                    }
                    // Grab data for flushing
                    if (clearAfter) {
                        toFlush.swap(this->componentDiagnostics);
                    } else {
                        toFlush = this->componentDiagnostics;
                    }
                }
                // Flush
                for (const auto &compDiag : toFlush) {
                    this->callbacks.onComponentFlush(compDiag);
                }
            }

            // Flush all grouped file diagnostics and component diagnostics
            void manualFlushAll(bool clearAll = true) {
                {
                    std::lock_guard<std::mutex> lock(this->hookMtx);
                    if (this->callbacks.onFileFlush != nullptr) {
                        for (const auto &[uri, state] : this->fileDiagnostics) {
                            if (this->mode == Mode::Eager || !state.diagnostics.empty()) {
                                this->callbacks.onFileFlush(uri, state.diagnostics);
                            }
                        }
                    }
                    if (clearAll) {
                        this->fileDiagnostics.clear();
                    }
                }
                this->flushComponentDiagnostics(clearAll);
            }

            // Stats
            [[nodiscard]] Stats getStats() {
                std::lock_guard<std::mutex> lock(this->hookMtx);
                return this->stats;
            }
            void resetStats() {
                std::lock_guard<std::mutex> lock(this->hookMtx);
                this->stats = Stats{};
            }
    };
}
