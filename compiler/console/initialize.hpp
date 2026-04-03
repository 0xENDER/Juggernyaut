/**
 * @brief
 * Handle CLI initialisation
**/

#pragma once

#include "../../core/common/headers.hpp"

// Basic C++ headers
#include <chrono>

namespace Console {
    namespace Internal {
        extern std::chrono::steady_clock::time_point start;

        // Handle CLI initialisation
        extern void initialize() ;
    }
}
