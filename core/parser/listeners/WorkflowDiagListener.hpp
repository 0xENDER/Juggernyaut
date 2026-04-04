/**
 * @brief
 * Session Workflow listeners
**/

#pragma once

#include "DiagnosticListener.hpp"

// Parser
#include "../Hooks.hpp"

namespace Parser {
    namespace Listeners {
        // Listen for syntax-related errors
        class JUG_PARSER_API WorkflowDiagListener : public DiagnosticListener {
            private:
                const Hooks &hooks;
                const Data::Store::SourceId srcId;
            public:
                // Constructors
                WorkflowDiagListener(const Hooks &parserHooks, Data::Store::SourceId sourceId)
                    : hooks(parserHooks), srcId(sourceId) {};

                void onSyntaxError(Diagnostics::Diagnostic diag) override;
                void onAmbiguity(Diagnostics::Diagnostic diag) override;
                void onAttemptingFullContext(Diagnostics::Diagnostic diag) override;
                void onContextSensitivity(Diagnostics::Diagnostic diag) override;
        };
    }
}
