/**
 * @brief
 * Lock-free monotonic progress tracker across front-end and back-end compilation phases
**/

#pragma once

#include "headers.hpp"

// C++ headers
#include <algorithm>

namespace Common::Progress {
    enum class PipelinePhase : uint8_t {
        Setup     = 0, // Config loading & dependency graph
        Parse     = 1, // CST + AST + dynamic import discovery
        Transform = 2, // AST lowering
        Semantic  = 3, // Type & symbol analysis
        IaRC      = 4, // Reachability & flow validation
        IRLower   = 5, // Back-end IR generation
        Optimize  = 6, // Back-end optimization passes
        Codegen   = 7, // Machine/object code emission
        Link      = 8, // Final binary/artifact linking
        COUNT     = 9
    };

    class ProgressTracker {
        public:
            static constexpr size_t PHASE_COUNT = static_cast<size_t>(PipelinePhase::COUNT);
            using UpdateCallback = std::function<void(int)>;

        private:
            struct PhaseSlot {
                std::atomic<uint32_t> weight{0};
                std::atomic<uint32_t> completed{0};
                std::atomic<uint32_t> total{0};
                std::atomic<bool> dynamicTotal{false};
                std::atomic<bool> finished{false};
            };

            std::array<PhaseSlot, PHASE_COUNT> phases{};
            std::atomic<int> highWaterPercent{-1}; // -1 = inactive
            std::atomic<bool> active{false};
            const UpdateCallback onUpdate;

            [[nodiscard]] static constexpr size_t toIndex(PipelinePhase phase) noexcept {
                return static_cast<size_t>(phase);
            }

            void publishHighWater(int candidate) {
                int prev = this->highWaterPercent.load(std::memory_order_relaxed);
                while (prev >= 0 && candidate > prev) {
                    if (this->highWaterPercent.compare_exchange_weak(
                            prev, candidate, std::memory_order_acq_rel, std::memory_order_relaxed)) {
                        if (this->onUpdate != nullptr) {
                            this->onUpdate(this->highWaterPercent.load(std::memory_order_acquire));
                        }
                        break;
                    }
                }
            }

            void recompute() {
                if (!this->active.load(std::memory_order_acquire)) {
                    return;
                }

                uint64_t totalWeight = 0;
                double weightedCompleted = 0.0;

                for (const PhaseSlot &slot : this->phases) {
                    const uint32_t slotWeight = slot.weight.load(std::memory_order_relaxed);
                    if (slotWeight == 0u) {
                        continue;
                    }
                    totalWeight += slotWeight;

                    if (slot.finished.load(std::memory_order_relaxed)) {
                        weightedCompleted += static_cast<double>(slotWeight);
                        continue;
                    }

                    const uint32_t done = slot.completed.load(std::memory_order_relaxed);
                    if (done == 0u) {
                        continue;
                    }

                    const uint32_t tot = slot.total.load(std::memory_order_relaxed);
                    double ratio = 0.0;
                    if (slot.dynamicTotal.load(std::memory_order_relaxed)) {
                        // Bounded asymptotic curve `done / (max(tot, done) + 1)`:
                        // approaches 1.0 during import discovery without hitting 1.0 until `finishPhase()`
                        const double denom = static_cast<double>(std::max(tot, done)) + 1.0;
                        ratio = static_cast<double>(done) / denom;
                    } else if (tot > 0u) {
                        ratio = std::min(1.0, static_cast<double>(done) / static_cast<double>(tot));
                    }

                    weightedCompleted += static_cast<double>(slotWeight) * ratio;
                }

                if (totalWeight == 0u) {
                    return;
                }

                const double rawPct = (weightedCompleted * 100.0) / static_cast<double>(totalWeight);
                const int clampedPct = std::clamp(static_cast<int>(rawPct), 0, 100);
                this->publishHighWater(clampedPct);
            }

        public:
            explicit ProgressTracker(UpdateCallback updateCallback = nullptr)
                : onUpdate(std::move(updateCallback)) {}

            ProgressTracker(const ProgressTracker&) = delete;
            ProgressTracker& operator=(const ProgressTracker&) = delete;

            // Configure phase weight (set weight = 0 to disable unused phases, e.g. in `--check` mode)
            void configurePhase(PipelinePhase phase, uint32_t phaseWeight, bool isDynamicDiscovery = false) noexcept {
                PhaseSlot &slot = this->phases[toIndex(phase)];
                slot.weight.store(phaseWeight, std::memory_order_relaxed);
                slot.dynamicTotal.store(isDynamicDiscovery, std::memory_order_relaxed);
                slot.completed.store(0u, std::memory_order_relaxed);
                slot.total.store(0u, std::memory_order_relaxed);
                slot.finished.store(false, std::memory_order_relaxed);
            }

            // Configure sensible defaults for Front-End (+ optional Back-End)
            void configureDefaultPipeline(bool includeBackEnd = true) noexcept {
                uint32_t m = (includeBackEnd) ? 1u : 2u;
                this->configurePhase(PipelinePhase::Setup,     5u * m,  true);
                this->configurePhase(PipelinePhase::Parse,     15u * m, true);
                this->configurePhase(PipelinePhase::Transform, 10u * m, false);
                this->configurePhase(PipelinePhase::Semantic,  15u * m, false);
                this->configurePhase(PipelinePhase::IaRC,      5u * m,  false);

                this->configurePhase(PipelinePhase::IRLower,   (includeBackEnd) ? 15u : 0u, false);
                this->configurePhase(PipelinePhase::Optimize,  (includeBackEnd) ? 15u : 0u, false);
                this->configurePhase(PipelinePhase::Codegen,   (includeBackEnd) ? 15u : 0u, false);
                this->configurePhase(PipelinePhase::Link,      (includeBackEnd) ? 5u  : 0u, false);
            }

            // Start a new build pass at 0%
            void begin() {
                for (PhaseSlot &slot : this->phases) {
                    slot.completed.store(0u, std::memory_order_relaxed);
                    if (slot.dynamicTotal.load(std::memory_order_relaxed)) {
                        slot.total.store(0u, std::memory_order_relaxed);
                    }
                    slot.finished.store(false, std::memory_order_relaxed);
                }
                this->highWaterPercent.store(0, std::memory_order_release);
                this->active.store(true, std::memory_order_release);

                if (this->onUpdate != nullptr) {
                    this->onUpdate(0);
                }
            }

            // Stop tracking (optionally snapping to 100% first)
            void end(bool snapToComplete = false) {
                if (snapToComplete) {
                    this->publishHighWater(100);
                }
                this->active.store(false, std::memory_order_release);
            }

            // Set known work count for deterministic stages (Transform, Semantic, Codegen, etc.)
            void setPhaseTotal(PipelinePhase phase, uint32_t totalUnits) {
                PhaseSlot &slot = this->phases[toIndex(phase)];
                slot.total.store(totalUnits, std::memory_order_relaxed);
                this->recompute();
            }

            // Register newly discovered work items in dynamic stages (e.g. Parse worklist)
            void addDynamicWork(PipelinePhase phase, uint32_t count = 1u) noexcept {
                this->phases[toIndex(phase)].total.fetch_add(count, std::memory_order_relaxed);
            }

            // Record completed work items in a stage
            void completeWork(PipelinePhase phase, uint32_t count = 1u) {
                this->phases[toIndex(phase)].completed.fetch_add(count, std::memory_order_relaxed);
                this->recompute();
            }

            // Convenience for 1-step dynamic tasks (e.g. skipped files or loaded configs)
            void completeDynamicWork(PipelinePhase phase, uint32_t count = 1u) {
                PhaseSlot &slot = this->phases[toIndex(phase)];
                slot.total.fetch_add(count, std::memory_order_relaxed);
                slot.completed.fetch_add(count, std::memory_order_relaxed);
                this->recompute();
            }

            // Mark an entire phase 100% complete (snaps slice to full weight even if 0 items ran)
            void finishPhase(PipelinePhase phase) {
                this->phases[toIndex(phase)].finished.store(true, std::memory_order_relaxed);
                this->recompute();
            }

            void finishFrontEnd() {
                this->phases[toIndex(PipelinePhase::Setup)].finished.store(true, std::memory_order_relaxed);
                this->phases[toIndex(PipelinePhase::Parse)].finished.store(true, std::memory_order_relaxed);
                this->phases[toIndex(PipelinePhase::Transform)].finished.store(true, std::memory_order_relaxed);
                this->phases[toIndex(PipelinePhase::Semantic)].finished.store(true, std::memory_order_relaxed);
                this->phases[toIndex(PipelinePhase::IaRC)].finished.store(true, std::memory_order_relaxed);
                this->recompute();
            }

            [[nodiscard]] int getPercent() const noexcept {
                return this->highWaterPercent.load(std::memory_order_acquire);
            }
    };
}
