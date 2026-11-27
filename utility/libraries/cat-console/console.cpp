/**
 * @brief
 * Enable uniform reports for both consoles and IDEs
**/

#include "include/console/console.hpp"

// C++ headers
#include <algorithm>

namespace Console {
    static std::mutex reportMtx;

    std::unique_lock<std::mutex> acquireReportLock() {
        return std::unique_lock<std::mutex>(reportMtx);
    }

    namespace Progress {
        static std::atomic<int> currentPercent{-1};

        void reset(int initialPercent) noexcept {
            currentPercent.store(std::clamp(initialPercent, 0, 100), std::memory_order_release);
        }

        void advancePercent(int percent) noexcept {
            const int clamped = std::clamp(percent, 0, 100);
            int prev = currentPercent.load(std::memory_order_relaxed);
            while (prev >= 0 && clamped > prev) {
                if (currentPercent.compare_exchange_weak(
                        prev, clamped, std::memory_order_release, std::memory_order_relaxed)) {
                    break;
                }
            }
        }

        void disable() noexcept {
            currentPercent.store(-1, std::memory_order_release);
        }

        int getPercent() noexcept {
            return currentPercent.load(std::memory_order_acquire);
        }
    }

    // Handle error throw statement
    static void throwError(std::string msg) {

        // Flush channels
        std::cout.flush();
        std::clog.flush();
        std::fflush(stdout);
        std::fflush(stderr);

        std::cerr << "\n" << Internal::color("[Internal Error] ", Console::Internal::Color::red)
            << Internal::color(msg, Console::Internal::Color::red)
            << Internal::color(BAD_CODE_OR_MEMORY_LEAKS,
                Console::Internal::Color::red)
            << "\n" << std::endl;

        throw std::runtime_error(msg);
    }

    // General compiler status
    namespace ProcessReport {
        int programStatus = 0; // Default: 0 (success)
        bool didSendReport = false;
    }

    // Report-specific status
    namespace IndividualReport {
        bool isNew = true;
        ReportType type = UNKNOWN_REPORT;
        std::string stage = "";
        std::stringstream messageStream;

        bool isContinuation = false;

        // Code-related report data
        std::string path = "";
        size_t startLine = 0;
        size_t startColumn = 0;
        size_t endLine = 0;
        size_t endColumn = 0;

        int code = 1;

        static void reset() {
            isNew = true;
            type = UNKNOWN_REPORT;
            stage = "";
            messageStream.str("");
            messageStream.clear();

            isContinuation = false;

            path = "";
            startLine = 0;
            startColumn = 0;
            endLine = 0;
            endColumn = 0;

            code = 1;
        }

        static void send() {
            if (IndividualReport::isNew) {
                throwError("Attempting to send an untouched/incorrectly initialised Comms::IndividualReport!");
            }
            if(!ProcessReport::didSendReport){
                ProcessReport::didSendReport = true;
            }
            Internal::Reports::print();
            reset();
        }
    }

    // Keep track of general report statistics
    namespace Statistics {
        int normalReports = 0;
        int warningReports = 0;
        int criticalReports = 0;
        int fatalReports = 0;
        int actionReports = 0;
        int debugReports = 0;
    }

    namespace ReportInternals {
        static void processReportControl(const ReportInput& arg) {
            if (std::holds_alternative<ReportAction>(arg)) {
                ReportAction value = std::get<ReportAction>(arg);
                if (value == START_REPORT){
                    if (!IndividualReport::isNew) {
                        throwError("Starting a new Comms::IndividualReport without ending the previous one!"); 
                    }
                    IndividualReport::isNew = false;
                } else if (value == END_REPORT) {
                    IndividualReport::send();
                } else {
                    throwError("Unknown Comms::ReportAction value!"); 
                }
            } else if (std::holds_alternative<ReportType>(arg)) {
                ReportType value = std::get<ReportType>(arg);

                if (!IndividualReport::isContinuation) {
                    switch (value) {
                    case NORMAL_REPORT:
                        Statistics::normalReports++;
                        break;
                    case WARNING_REPORT:
                        Statistics::warningReports++;
                        break;
                    case CRITICAL_REPORT:
                        Statistics::criticalReports++;
                        ProcessReport::programStatus = 1;
                        break;
                    case FATAL_REPORT:
                        Statistics::fatalReports++;
                        ProcessReport::programStatus = 2;
                        break;
                    case ACTION_REPORT:
                        Statistics::actionReports++;
                        break;
                    case DEBUG_REPORT:
                        Statistics::debugReports++;
                        break;
                
                    default:
                        throwError("Unknown Comms::ReportType value!");
                    }
                }

                IndividualReport::type = value;
            } else {
                throwError("Unknown Comms::report control argument type!");
            }
        }

        static void processReportInput(const ReportInput& arg) {
            if (IndividualReport::isNew) {
                throwError("Attempting to inject a value to an uninitialised Comms::report!");
            }
            bool isMessageBody = true;
            std::stringstream text;
            if (std::holds_alternative<std::string>(arg)) {
                std::string value = std::get<std::string>(arg);
                text << value;
            } else if (std::holds_alternative<size_t>(arg)) {
                size_t value = std::get<size_t>(arg);
                text << value;
            } else {
                throwError("Unknown Comms::report input argument type!"); 
            }

            if (isMessageBody) {
                IndividualReport::messageStream << text.str();
            }
        }
    }

    template <typename... Types, typename Variant>
    static bool holdsAlternatives(const Variant& variant) {
        return (std::holds_alternative<Types>(variant) || ...);
    }

    void report(const ReportInputs& args) {
        for (const auto& arg : args) {
            if (holdsAlternatives<REPORT_BASIC_TYPES>(arg)) {
                ReportInternals::processReportControl(arg);
            } else if (holdsAlternatives<REPORT_INPUT_TYPES>(arg)) {
                ReportInternals::processReportInput(arg);
            } else {
                throwError("unknown Comms::report argument type!"); 
            }
        }
    }

    void runtimeTracking() {
        Internal::startClock();
    };

    void initalize(const InitializationData &data) {
        Internal::initialize(data); //TMP
    }

    static bool isFinalized = false;
    bool minimalProtocolFinalization = false;

    void finalize(uint32_t activeSrcs) {
        Progress::disable();
        Console::Optimization::safetyCheck();

        if (isFinalized) {
            throwError("Detecting multiple protocol finalisation attempts!");
        }
        isFinalized = true;

        Internal::finalize(activeSrcs); //TMP

        if(ProcessReport::didSendReport && !IndividualReport::isNew){
            throwError("Detected an unfinished report!");
        }
    }
}
