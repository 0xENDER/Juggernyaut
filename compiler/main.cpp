/**
 * @brief
 * Juggernyaut Compiler
**/

// Basic C++ headers
#include <cstdlib>
#include <fstream>

// Common headers
#include "common/headers.hpp"
#include "common/crashBreadcrumb.hpp"
#include "common/progressTracker.hpp"

#include "console/console.hpp"

// Compiler Store
#include "store/Documents.hpp"

// Diagnostics & Front-End Driver
#include "../core/data/diagnostics/DiagnosticsManager.hpp"
#include "../core/driver/JugDriver.hpp"

// Base
#include "base/config.hpp"
#include "base/info.hpp"

using Diagnostic = Data::Diagnostics::Diagnostic;
using DiagnosticsManager = Data::Diagnostics::DiagnosticsManager<Diagnostic>;
using Phase = Common::Progress::PipelinePhase;

namespace Helper {
    Console::ReportType getReportType(Data::Diagnostics::Severity severity) {
        switch (severity) {
        case Data::Diagnostics::Severity::Error:
            return Console::CRITICAL_REPORT;
        case Data::Diagnostics::Severity::Warning:
            return Console::WARNING_REPORT;
        case Data::Diagnostics::Severity::Info:
            return Console::NORMAL_REPORT;
        case Data::Diagnostics::Severity::Hint:
            return Console::NORMAL_REPORT;
        default:
            return Console::UNKNOWN_REPORT;
        }
    }

    const char* getStageName(int diagCode) noexcept {
        switch (std::abs(diagCode) / 100'000) {
        case 1:  return "Lexer";
        case 2:  return "Parser";
        case 3:  return "Transformer";
        case 4:  return "Semantic";
        case 5:  return "IaRC";
        case 6:  return "Session";
        case 7:  return "Driver";
        default: return "?Unknown Stage?";
        }
    }

    void printDiagnosticToConsole(const std::string &uri, const Diagnostic &diag) {
        auto reportGuard = Console::acquireReportLock();

        if (!uri.empty()) {
            Console::IndividualReport::path = uri;
            Console::IndividualReport::startLine = diag.range.start.line;
            Console::IndividualReport::startColumn = diag.range.start.character;
            Console::IndividualReport::endLine = diag.range.end.line;
            Console::IndividualReport::endColumn = diag.range.end.character;
        }

        Console::IndividualReport::code = diag.code;
        Console::IndividualReport::stage = getStageName(diag.code);

        REPORT(Console::START_REPORT, getReportType(diag.severity), diag.message, Console::END_REPORT);
    }

    void bindFrontEndProgress(Driver::Hooks &hooks, Common::Progress::ProgressTracker &tracker) {
        hooks.driver.onEnvironmentLoad << [&tracker](const Data::Host::File *file) {
            if (file != nullptr) {
                tracker.completeDynamicWork(Phase::Setup, 1u);
            }
        };
        hooks.unitSources.onContextStart << [&tracker](const Data::Host::File *file) {
            if (file != nullptr) {
                tracker.addDynamicWork(Phase::Parse, 1u);
            }
        };
        hooks.unitSources.onParserContextEnd << [&tracker](const Data::Host::File *file) {
            if (file != nullptr) {
                tracker.completeWork(Phase::Parse, 1u);
            }
        };
        hooks.unitSources.onContextSkip << [&tracker](const Data::Host::File *file) {
            if (file != nullptr) {
                tracker.completeDynamicWork(Phase::Parse, 1u);
            }
        };
        hooks.driver.onRunFinished << [&tracker]() {
            tracker.finishFrontEnd();
        };
    }
}

int main(int argc, const char *argv[]) {
    Common::Debug::installCrashBreadcrumbHandler();
    Console::runtimeTracking();

    // Initialise communications protocol
    // (Basically allowing the default protocol to take effect)
    if (!Base::InitialConfigs::Technical::shouldSkipDefaultInitialization(argc, argv)) {
        // This is done to allow flags like --version to function normally
        Console::initalize({
            "Juggernyaut Compiler",
            Base::Info::version,
            Base::Info::build,
            Base::Info::arch,
            Base::Info::osName,
            Base::Info::osVersion
        });
    }

    // Update initial configurations
    if (!Base::InitialConfigs::updateUsingArgs(argc, argv)) {
        // This process failed!
        if (Console::Statistics::fatalReports != 0) {
            auto reportGuard = Console::acquireReportLock();
            REPORT(Console::START_REPORT, Console::FATAL_REPORT, "terminating program due to a Base::InitialConfigs error!",
                BAD_CODE_OR_MEMORY_LEAKS,
                Console::END_REPORT);
        }

        // End the program
        Console::finalize();
        return Console::ProcessReport::programStatus;
    }

    // Check if other delayed actions are allowed to run
    if (Base::InitialConfigs::Technical::terminateAfterArgs) {
        // End the program
        Console::finalize();
        return Console::ProcessReport::programStatus;
    }

    // Set up the driver
    auto docs = std::make_unique<Store::Documents>();
    Driver::JugDriver::HostConfigs host;
    host.vfs = docs.get();
    Driver::JugDriver::DriverConfigs configs;
    Driver::JugDriver::JugDriver driver(host, configs);

    // Subscribe ProgressTracker FIRST so `[XXX%]` advances before stage diagnostics flush
    Common::Progress::ProgressTracker progressTracker([](int pct) {
        Console::Progress::advancePercent(pct);
    });
    progressTracker.configureDefaultPipeline(true); // (Include future back-end values)
    Helper::bindFrontEndProgress(driver.configs.hooks, progressTracker);

    // Subscribe DiagnosticsManager (`Funnelled` when hotDiagnostics is set, `Default` otherwise)
    const auto diagMode = Base::InitialConfigs::Technical::hotDiagnostics
        ? Data::Diagnostics::ManagerMode::Funnelled
        : Data::Diagnostics::ManagerMode::Default;
    DiagnosticsManager diagManager(
        driver.configs.hooks,
        DiagnosticsManager::ManagerCallbacks{
            .onFileFlush = [](const std::string &uri, const std::vector<Diagnostic> &diags) {
                for (const Diagnostic &d : diags) {
                    Helper::printDiagnosticToConsole(uri, d);
                }
            },
            .onComponentFlush = [](const Diagnostic &d) {
                Helper::printDiagnosticToConsole("", d);
            }
        },
        diagMode
    );

    // Activate `[  0%]` progress tracking for pipeline execution
    Console::Progress::reset(0);
    progressTracker.begin();

    // Add input files
    if (Base::InitialConfigs::Input::entryPaths.empty()) {
        const std::string configUri = driver.lookupProject(driver.host.vfs->cwd());
        if (configUri.empty()) {
            {
                auto reportGuard = Console::acquireReportLock();
                REPORT(Console::START_REPORT, Console::CRITICAL_REPORT, "couldn't find a 'jug.toml' configuration file!", Console::END_REPORT);
            }
            progressTracker.end(false);
            Console::finalize();
            return 1;
        }
        driver.addProject(configUri);
    } else {
        driver.processPaths(Base::InitialConfigs::Input::entryPaths);
    }

    // Check for requested termination
    if (Base::InitialConfigs::Technical::terminateAfterActions) {
        diagManager.manualFlushAll(true);
        progressTracker.end(false);
        Console::finalize(diagManager.getStats().filesVisited());
        return Console::ProcessReport::programStatus;
    }

    // Run the front-end
    driver.run();

    // Abort before back-end if front-end encountered errors
    const DiagnosticsManager::Stats frontEndStats = diagManager.getStats();
    if (frontEndStats.hasErrors()) {
        progressTracker.end(false);
        Console::finalize(frontEndStats.filesVisited());
        return Console::ProcessReport::programStatus;
    }

    // Handle back-end logic
    // ...

    // Dummy progress
    // (As back-end passes are implemented, `setPhaseTotal(...)` + `completeWork(...)` calls
    // must be implemented in a new Helper::bindBackEndProgress function!)
    progressTracker.finishPhase(Phase::IRLower);
    progressTracker.finishPhase(Phase::Optimize);
    progressTracker.finishPhase(Phase::Codegen);
    progressTracker.finishPhase(Phase::Link);
    progressTracker.end(true);

    // End the program
    Console::finalize(frontEndStats.filesVisited());
    return Console::ProcessReport::programStatus;
}
