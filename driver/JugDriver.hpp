/**
 * @brief 
 * Front-End Driver
**/

#pragma once

#include "common/headers.hpp"
#include "dynamic.hpp" // JUG_DRIVER_API

// Host/System
#include "../core/data/host/VFS.hpp"

// Sessions
#include "../core/session/session.hpp"

#include "Unit.hpp"
#include "Hooks.hpp"

namespace Driver {
    namespace JugDriver {
        struct HostConfigs {
            Data::Host::VFS *vfs = nullptr; // Virtual File System
        };

        struct DriverConfigs {
            std::vector<std::string> input; // jug.toml/*.jug files/directories
            const std::string librariesPath = ""; // Absolute path to the root libraries directory!
            Hooks hooks;
        };

        class JUG_DRIVER_API JugDriver {
            private:
                std::unordered_map<UnitId, std::unique_ptr<Unit>> sessions;
                std::vector<UnitId> entryPoints;
                HostConfigs host;
                DriverConfigs configs;

            public:
                UnitId lastID = 10;

                JugDriver(HostConfigs hostConfigs, DriverConfigs driverConfigs) : host(hostConfigs), configs(driverConfigs) {};
                JugDriver& operator=(const JugDriver&) = delete;
                virtual ~JugDriver() = default;

                void run() ;
        };
    }
}
