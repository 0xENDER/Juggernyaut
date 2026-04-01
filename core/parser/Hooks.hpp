/**
 * @brief 
 * Parser hooks
**/

#pragma once

#include "../common/headers.hpp"
#include "dynamic.hpp" // JUG_PARSER_API

// Basic C++ headers
#include <functional>

#include "listeners/DiagnosticListener.hpp"

namespace Parser {
    typedef std::function<void(const std::string)> TokenReport;
    typedef std::function<void(const std::string)> TreeReport;

    struct Hooks {
        // [TokenReport]
        // Args: <token_text> (const std::string)
        TokenReport onTokenDetected = nullptr;
        // [TreeReport]
        // Args: <AST_text> (const std::string)
        TreeReport onTreeGenerated = nullptr;
    };
}
