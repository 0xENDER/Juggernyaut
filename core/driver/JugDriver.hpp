/**
 * @brief 
 * Front-End Driver
**/

#pragma once

#include "common/headers.hpp"
#include "dynamic.hpp" // JUG_DRIVER_API

// Host/System
#include "../data/host/VFS.hpp"

// Sessions
#include "../session/session.hpp"

#include "Unit.hpp"
#include "Hooks.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251) // Suppress DLL interface warning for STL types
#endif

namespace Driver {
    namespace JugDriver {
        struct HostConfigs {
            Data::Host::VFS *vfs = nullptr; // Virtual File System
        };

        struct DriverConfigs {
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
                virtual ~JugDriver() = default;

                // Explicitly delete copy constructor and assignment!
                JugDriver(const JugDriver&) = delete;
                JugDriver& operator=(const JugDriver&) = delete;
                // Explicitly allow moving
                //JugDriver(JugDriver&&) noexcept = default;
                //JugDriver& operator=(JugDriver&&) noexcept = default;

                void run() ;

                void processPaths(std::vector<std::string> &paths) ;

                void addProject(const std::string &path) ;
                void addStrayFile(const std::string &path) ;
        };
    }
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
