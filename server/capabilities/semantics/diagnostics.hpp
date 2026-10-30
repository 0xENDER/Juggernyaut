/**
 * @brief
 * Manage code diagnostics
 * (triggered by document content changes)
**/

#pragma once

// Common headers
#include "common/headers.hpp"

// Parser
#include "../../../core/parser/parser.hpp"
//#include "../../../core/parser/listeners/DiagnosticListener.hpp"

// Front-End Driver
#include "./../../../core/driver/JugDriver.hpp"

// lsp-framework
#include "../../lspFramework.hpp"

namespace Capabilities {
    namespace Semantics {
        extern void setupGlobalDiagnostics(lsp::ServerEndpoint &endpoint, Driver::JugDriver::JugDriver &driver) ;
    }
}
