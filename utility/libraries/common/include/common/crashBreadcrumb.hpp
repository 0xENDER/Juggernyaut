/**
 * @brief
 * Per-thread active compiler task breadcrumb for fatal crash (ICE) diagnostics
**/

#pragma once

#include "../../dynamic.hpp" // JUG_COMMON_API

namespace Common::Debug {
    // Borrowed pointers describing what the calling thread is currently executing.
    // `stage` should be a string literal; `detail` must remain valid for the scope's lifetime
    // (e.g. `file->canonical.c_str()`, which is immutable in VFS).
    struct ActiveTask {
        const char *stage = "Idle";
        const char *detail = nullptr;
    };

    // Cross-DLL accessor for the calling thread's breadcrumb slot
    [[nodiscard]] extern JUG_COMMON_API ActiveTask& currentActiveTask() noexcept;

    // Installs process-wide OS crash & `std::terminate` handlers (safe to call multiple times)
    extern JUG_COMMON_API void installCrashBreadcrumbHandler() noexcept;

    // Zero-allocation RAII guard that records an active stage/target and restores the previous one on exit
    class TaskScope {
        private:
            ActiveTask *slot = nullptr;
            ActiveTask previous{};

        public:
            explicit TaskScope(const char *stageName, const char *detailText = nullptr) noexcept
                : slot(&currentActiveTask()), previous(*this->slot) {
                this->slot->stage = (stageName != nullptr) ? stageName : "Unknown";
                this->slot->detail = detailText;
            }
            ~TaskScope() noexcept {
                *(this->slot) = this->previous;
            }

            TaskScope(const TaskScope&) = delete;
            TaskScope& operator=(const TaskScope&) = delete;

            // Updates the current sub-stage in place (2 pointer stores, no TLS lookup)
            void update(const char *stageName, const char *detailText = nullptr) noexcept {
                this->slot->stage = (stageName != nullptr) ? stageName : "Unknown";
                this->slot->detail = detailText;
            }
    };
}
