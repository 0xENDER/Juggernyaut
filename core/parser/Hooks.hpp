/**
 * @brief 
 * Parser hooks
**/

#pragma once

#include "common/headers.hpp"
#include "dynamic.hpp" // JUG_PARSER_API

// Diagnostics
#include "../diagnostics/Diagnostic.hpp"

namespace Parser {
    typedef std::function<void()> StageCall;

    // typedef std::function<Data::Store::Source(const std::string)> SourceRequest;

    struct Hooks {
        // [StageCall]
        StageCall onContextSkip = nullptr; // Context has already been processed
        StageCall onContextStart = nullptr; // Fresh check started
        StageCall onLexerContextEnd = nullptr; // Lexer checks ended
        StageCall onContextEnd = nullptr; // Check ended

        // [Diagnostic Events]
        // Args: <diagnostics_obj> (Diagnostics::Diagnostic)
        Diagnostics::Event onDiagnostic = nullptr;

        //// [SourceRequest]
        //// Args: <file_uri> (const std::string)
        //// Returns: <file_source_obj> (Data::Store::Source)
        // SourceRequest onSourceRequest = nullptr;
    };
}
