/**
 * @brief 
 * Front-End Driver Hooks
**/

#pragma once

#include "common/headers.hpp"
#include "dynamic.hpp" // JUG_DRIVER_API

#include "../diagnostics/Diagnostic.hpp"
#include "../data/store/types.hpp"

#include "Unit.hpp"

namespace Driver {
    using Diagnostic = Diagnostics::Diagnostic;

    // Unit's source-level types
    using UnitSourceId = Data::Store::SourceId;

    using UnitSourceEvent = std::function<void(const Unit&, const UnitSourceId&)>;
    using UnitSourceStringEvent = std::function<void(const Unit&, const std::string&)>;
    using UnitSourceU8tStringEvent = std::function<void(const Unit&, const uint8_t&, const std::string&)>;
    using UnitSourceDiagnosticEvent = std::function<void(const Unit&, const Diagnostic&)>;

    // Parser events
    struct UnitSourceParserEvents {
        // Stage
        UnitSourceEvent onContextSkip = nullptr;
        UnitSourceEvent onContextStart = nullptr;
        UnitSourceEvent onLexerContextEnd = nullptr;
        UnitSourceEvent onContextEnd = nullptr;

        // ANTL4
        UnitSourceU8tStringEvent onANTLRTokenDetected = nullptr;
        UnitSourceStringEvent onANTLRTreeGenerated = nullptr;

        // Diagnostics
        UnitSourceDiagnosticEvent onSyntaxError = nullptr;
        UnitSourceDiagnosticEvent onAmbiguity = nullptr;
        UnitSourceDiagnosticEvent onAttemptingFullContext = nullptr;
        UnitSourceDiagnosticEvent onContextSensitivity = nullptr;
    };

    // Unit's source-level hooks
    struct UnitSourceHooks {
        // Parser events
        UnitSourceParserEvents parser;
    };

    // Unit-level hooks
    struct UnitHooks {
        // ...
    };

    using DriverEvent = std::function<void(const Diagnostic&)>;

    // Driver-level hooks
    struct DriverHooks {
        DriverEvent onEntryError = nullptr;
    };

    struct Hooks {
        DriverHooks driver;
        UnitHooks units;
        UnitSourceHooks unitSources;
    };
}
