/**
 * @brief 
 * Front-End Driver
**/

#pragma once

#include "common/headers.hpp"
#include "dynamic.hpp" // JUG_DRIVER_API

// Sessions
#include "../session/session.hpp"

namespace Driver {
    using UnitId = uint32_t;
    using UnitSessionPtr = std::unique_ptr<Session::Session>;

    struct Unit {
        const std::string rootDir = "";
        UnitSessionPtr session = nullptr;
    };
}
