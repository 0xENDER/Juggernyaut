/**
 * @brief 
 * Session-specific hooks
**/

#pragma once

#include "common/headers.hpp"
#include "dynamic.hpp" // JUG_SESSION_API

// Diagnostics
#include "../diagnostics/Diagnostic.hpp"

namespace Session {
    typedef std::function<void()> StageCall;

    struct Hooks {
        // [StageCall]
        StageCall onRunFinished = nullptr; // The Session is done running!
    };
}
