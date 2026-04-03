/**
 * @brief
 * Juggernyaut Compiler
**/

// Basic C++ headers
#include <fstream>

// Common headers
#include "../core/common/headers.hpp"
#include "../core/common/debug.hpp"

#include "console/console.hpp"

// Parser
// #include "../core/parser/parser.hpp"

// Diagnostics
#include "../core/diagnostics/Diagnostic.hpp"
#include "../core/diagnostics/antlr.hpp"

// Store
#include "store/FileStore.hpp"

// Session
#include "../core/session/session.hpp"

// Base
#include "base/config.hpp"
#include "base/info.hpp"

int main(int argc, const char *argv[]) {
    // Test for memory leaks
    Common::CrtDebug::initiateCrtMemoryChecks();

    // Initalise communications protocol
    // (Basiclly allowing the default protocol to take effect)
    if (!Base::InitialConfigs::Technical::shouldSkipDefaultInitialization(argc, argv)) {
        // This is done to allow flags like --version to function normally
        Console::initalize();
    }

    // Update initial configurations
    if(!Base::InitialConfigs::updateUsingArgs(argc, argv)){
        // This process failed!
        if (Console::Statistics::fatalReports == 0) {
            REPORT(Console::START_REPORT, Console::FATAL_REPORT, "Terminating program due to a Base::InitialConfigs error!",
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

    class ReportListener : public Parser::Listeners::DiagnosticListener {
        private:
            const char* stage; // "Lexer" or "Parser"
            static const Console::ReportType getReportType(Diagnostics::Severity severity) {
                switch (severity) {
                case Diagnostics::Severity::Error:
                    return Console::CRITICAL_REPORT;
                case Diagnostics::Severity::Warning:
                    return Console::WARNING_REPORT;
                case Diagnostics::Severity::Info:
                    return Console::NORMAL_REPORT;
                case Diagnostics::Severity::Hint:
                    return Console::NORMAL_REPORT;
                default:
                    return Console::UNKNOWN_REPORT;
                }
            }
        public:
            // Constructors
            ReportListener() = default;

            // ANTLR4 functions
            void onSyntaxError(Diagnostics::Diagnostic diag) override {
                REPORT(Console::END_REPORT);

                // Get the position
                Console::IndividualReport::startLine = diag.range.start.line;
                Console::IndividualReport::startColumn = diag.range.start.character;
                Console::IndividualReport::endLine = diag.range.end.line;
                Console::IndividualReport::endColumn = diag.range.end.character;

                // Get the token
                std::string tokenText = "?<N/A>?";

                Console::ReportType reportType = getReportType(diag.severity);

                // Update stage data
                int stageId = std::floor(diag.code / 100000);
                if (stageId == 1) {
                    Console::IndividualReport::stage = "Lexer";
                } else if (stageId == 2) {
                    Console::IndividualReport::stage = "Parser";
                } else {
                    Console::IndividualReport::stage = "?Unknown Stage?";
                }

                // Report the error
                REPORT(Console::START_REPORT, reportType,
                    diag.message, " (Token Text: '", tokenText, "')",
                    Console::END_REPORT);

                REPORT(Console::START_REPORT, Console::DEBUG_REPORT, "\n");
            }
            void onAmbiguity(Diagnostics::Diagnostic diag) override {
                REPORT(Console::START_REPORT, getReportType(diag.severity), diag.message, " (source: ",
                    diag.range.start.line, ":", diag.range.start.character ," to  " ,
                    diag.range.end.line, ":", diag.range.end.character, ")", Console::END_REPORT);
            }
    };

    Session::ParserConfigs parserConfigs;
    Session::Configs sessionConfigs = {
        ,
        parserConfigs
    };

    Session::ParserHooks parserHooks;
    Session::Hooks sessionHooks = {
        parserHooks
    };

    Store::FileStore fileStore;

    Session::Session session = {
        sessionConfigs,
        sessionHooks,
        fileStore
    };

    // Debug action
    if (Base::InitialConfigs::Debug::Parser::activateAntlrSyntaxTest) {
        parserHooks.onParserContextStart = [](){
            REPORT(Console::START_REPORT, Console::DEBUG_REPORT, "Tokens: \n");
        };
        parserHooks.onANTLRTokenDetected = [](const std::string &tokenText){
            REPORT(tokenText, "\n");
        };
        parserHooks.onANTLRTreeGenerated = [](const std::string &treeText){
            REPORT(Console::START_REPORT, Console::DEBUG_REPORT, "Parse Tree: \n", treeText, Console::END_REPORT);
        };
        parserHooks.onParserContextEnd = [](){
            REPORT(Console::END_REPORT);
        };
    }

    // Check for requested termination
    if (Base::InitialConfigs::Technical::terminateAfterActions) {
        // End the program
        Console::finalize();
        return Console::ProcessReport::programStatus;
    }

    // Begin the actual work here...
    Session::initiate(session);
    
    // Handle memory check results
    if(Common::CrtDebug::processCrtMemoryReports()){
        // Exist with an error on memory leaks!
        REPORT(Console::START_REPORT, Console::CRITICAL_REPORT,
            "Terminating program due to detected memory errors! Please contact the developers of Juggernyaut!",
            Console::END_REPORT);
        return 1;
    }

    // End the program
    Console::finalize();
    return Console::ProcessReport::programStatus;
}
