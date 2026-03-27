/**
 * @brief
 * Lexer & parser error listeners
**/

#include "errors.hpp"

#include "../antlr4.hpp"

#include "../../common/utility.hpp"

//#include "../../comms/comms.hpp"

namespace Parser {
    namespace Listeners {
        // Track errors
        bool errorsDetected = false;

        // Listen for syntax-related errors
        void ErrorListener::syntaxError(antlr4::Recognizer *recognizer, antlr4::Token *offendingSymbol, size_t line,
            size_t charPositionInLine, const std::string &msg, std::exception_ptr e) {

            /*if (e) {
                if (is_type<const antlr4::LexerNoViableAltException *>(&e)) {
                    // Lexer: Unrecognized character
                } else if (is_type<const antlr4::InputMismatchException *>(&e)) {
                    // Parser: Input mismatch
                } else if (is_type<const antlr4::NoViableAltException *>(&e)) {
                    // Parser: No viable alternative
                } else if (is_type<const antlr4::FailedPredicateException *>(&e)) {
                    // Semantics: Failed predicate
                } else if (is_type<const antlr4::RecognitionException *>(&e)) {
                    // Parser: Generic/Custom fallback
                }
            } else {
                if (msg.find("missing") != std::string::npos) {
                    // Parser: Missing token
                } else if (msg.find("extraneous") != std::string::npos) {
                    // Parser: Extraneous token
                } else {
                    // Parser: General auto-recovery
                }
            }*/
            errorsDetected = true;
        }

        // TMP
        void ErrorListener::reportAmbiguity(antlr4::Parser *recognizer, const antlr4::dfa::DFA &dfa, size_t startIndex,
            size_t stopIndex, bool exact, const antlrcpp::BitSet &ambigAlts, antlr4::atn::ATNConfigSet *configs) {

            /*REPORT(Comms::START_REPORT, Comms::CRITICAL_REPORT, "Ambiguity reported from index ",
                startIndex ," to index " , stopIndex, Comms::END_REPORT);*/
            errorsDetected = true;
        }
        void ErrorListener::reportAttemptingFullContext(antlr4::Parser *recognizer, const antlr4::dfa::DFA &dfa, size_t startIndex,
            size_t stopIndex, const antlrcpp::BitSet &conflictingAlts, antlr4::atn::ATNConfigSet *configs) {

            // TMP
            /*REPORT(Comms::START_REPORT, Comms::CRITICAL_REPORT, "Attempting full context reported from index ",
                startIndex ," to index " , stopIndex, Comms::END_REPORT);*/
            errorsDetected = true;
        }
        void ErrorListener::reportContextSensitivity(antlr4::Parser *recognizer, const antlr4::dfa::DFA &dfa, size_t startIndex,
            size_t stopIndex, size_t prediction, antlr4::atn::ATNConfigSet *configs) {

            // TMP
            /*REPORT(Comms::START_REPORT, Comms::CRITICAL_REPORT, "Context sensitivity reported from index ",
                startIndex ," to index " , stopIndex, Comms::END_REPORT);*/
            errorsDetected = true;
        }
    }
}
