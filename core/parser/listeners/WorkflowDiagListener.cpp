/**
 * @brief
 * Session Workflow listeners
**/

#include "WorkflowDiagListener.hpp"

namespace Parser {
    namespace Listeners {
        void WorkflowDiagListener::onSyntaxError(Diagnostics::Diagnostic diag) {
            diag.sourceId = this->srcId;
            const ParserDiagEvent &call = this->hooks.onSyntaxError;
            if (call != nullptr) {
                call(diag);
            }
        }
        void WorkflowDiagListener::onAmbiguity(Diagnostics::Diagnostic diag) {
            diag.sourceId = this->srcId;
            const ParserDiagEvent &call = this->hooks.onAmbiguity;
            if (call != nullptr) {
                call(diag);
            }
        }
        void WorkflowDiagListener::onAttemptingFullContext(Diagnostics::Diagnostic diag) {
            diag.sourceId = this->srcId;
            const ParserDiagEvent &call = this->hooks.onAttemptingFullContext;
            if (call != nullptr) {
                call(diag);
            }
        }
        void WorkflowDiagListener::onContextSensitivity(Diagnostics::Diagnostic diag) {
            diag.sourceId = this->srcId;
            const ParserDiagEvent &call = this->hooks.onContextSensitivity;
            if (call != nullptr) {
                call(diag);
            }
        }
    }
}