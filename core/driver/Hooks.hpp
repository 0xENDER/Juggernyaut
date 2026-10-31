/**
 * @brief 
 * Front-End Driver Hooks
**/

#pragma once

#include "common/headers.hpp"
#include "dynamic.hpp" // JUG_DRIVER_API

#include "../diagnostics/Diagnostic.hpp"

#include "Unit.hpp"

namespace Driver {
    using Diagnostic = Diagnostics::Diagnostic;
    using UnitSourceId = Data::Host::FileId;

    using GenericEvent = std::function<void()>;
    using FileEvent = std::function<void(const Data::Host::File*)>;
    using UnitSourceEvent = std::function<void(const JugDriver::Unit*, const UnitSourceId&)>;
    // using UnitSourceStringEvent = std::function<void(const Unit&, const std::string&)>;
    // using UnitSourceU8tStringEvent = std::function<void(const Unit&, const uint8_t&, const std::string&)>;

    // Unit's source-level hooks
    struct UnitSourceHooks {
        // Source stages
        UnitSourceEvent onContextSkip = nullptr;
        UnitSourceEvent onContextStart = nullptr;
        UnitSourceEvent onLexerContextEnd = nullptr;
        UnitSourceEvent onParserContextEnd = nullptr;
        UnitSourceEvent onContextEnd = nullptr;
    };

    // Unit-level hooks
    struct UnitHooks {
        // ...
    };

    using DriverEvent = std::function<void(const Diagnostic&)>;

    // Driver-level hooks
    struct DriverHooks {
        Diagnostics::Event onDiagnostic = nullptr; // ALL Diagnostics go through this function!
        GenericEvent onRunFinished = nullptr;
        FileEvent onEnvironmentLoad = nullptr;
        FileEvent onEnvironmentChange = nullptr;
    };

    struct Hooks {
        DriverHooks driver;
        UnitHooks units;
        UnitSourceHooks unitSources;
    };
}
