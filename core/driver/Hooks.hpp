/**
 * @brief 
 * Front-End Driver Hooks
**/

#pragma once

#include "common/headers.hpp"
#include "common/multicast.hpp"
#include "dynamic.hpp" // JUG_DRIVER_API

#include "../data/diagnostics/Diagnostic.hpp"

namespace Driver {
    using Diagnostic = Data::Diagnostics::Diagnostic;
    using UnitSourceId = Data::Host::FileId;

    // WARING: For the sake of efficiency, 'Common::Multicast' operations are
    // limits to generic & file events!
    using GenericEvent = Common::Multicast<Data::Diagnostics::GenericEvent>;
    using DriverEvent = Data::Diagnostics::Event;
    using FileEvent = Common::Multicast<Data::Diagnostics::FileEvent>;
    // using UnitSourceStringEvent = std::function<void(const Unit&, const std::string&)>;
    // using UnitSourceU8tStringEvent = std::function<void(const Unit&, const uint8_t&, const std::string&)>;

    // Unit's source-level hooks
    struct UnitSourceHooks {
        // Source stages
        FileEvent onContextSkip = nullptr;
        FileEvent onContextStart = nullptr;
        FileEvent onParserContextEnd = nullptr;
        FileEvent onContextEnd = nullptr;
        FileEvent onContextDetached = nullptr;
    };

    // Unit-level hooks
    struct UnitHooks {
        FileEvent onContextQueued = nullptr;
    };

    // Driver-level hooks
    struct DriverHooks {
        // General
        DriverEvent onDiagnostic = nullptr; // ALL Diagnostics go through this function!
        GenericEvent onRunFinished = nullptr;
        // Setup
        FileEvent onEnvironmentLoad = nullptr;
        FileEvent onEnvironmentChange = nullptr;
        GenericEvent onSetupStart = nullptr;
        GenericEvent onSetupFinished = nullptr;
        // ...
    };

    struct Hooks {
        DriverHooks driver;
        UnitHooks units;
        UnitSourceHooks unitSources;
    };
}
