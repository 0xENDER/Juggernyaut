/**
 * @brief 
 * Front-End Driver
**/

#pragma once

#include "common/headers.hpp"
#include "dynamic.hpp" // JUG_DRIVER_API

// Host/System
#include "../data/host/VFS.hpp"

// Environment data
#include "environment.hpp"

// Sessions
#include "../session/session.hpp"

#include "Unit.hpp"
#include "Hooks.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251) // Suppress DLL interface warning for STL types
#endif

// TO-DO: Replace unnecessary unordered maps with vectors!

namespace Driver {
    namespace JugDriver {
        using SetupId = Environment::EnvironmentId;
        using Setup = Environment::Environment;

        struct HostConfigs {
            Data::Host::VFS *vfs = nullptr; // Virtual File System
        };

        struct DriverConfigs {
            const std::string librariesPath = ""; // Absolute path to the root libraries directory!
            Hooks hooks;
        };

        class JUG_DRIVER_API JugDriver {
            public:
                // Provided Configs
                HostConfigs host;
                DriverConfigs configs;
            private:
                // Internal variables
                std::unordered_map<SetupId, std::unique_ptr<Setup>> setups; // TO-DO: Implement setups loadup logic!
                std::unordered_map<UnitId, std::unique_ptr<Unit>> sessions;
                // TO-DO: Phase out SourceStore in favour of a global VFS
                std::vector<UnitId> entryPoints;

                bool isBusy = false;
                std::vector<std::string> pendingPaths;

                // Internal
                void processPendingPaths() ;

                // Lifetime Management (Internal)
                void refreshSetups() ; // Checks jug.toml files for updates
                bool refreshDependencyGraph() ; // Updates dependency graph for parallel processing
            public:
                // NOTE: When the driver is busy(), never change the state of the inputed variables or their children!
                JugDriver(HostConfigs hostConfigs, DriverConfigs driverConfigs) : host(hostConfigs), configs(driverConfigs) {};
                virtual ~JugDriver() = default;

                // Explicitly delete copy constructor and assignment!
                JugDriver(const JugDriver&) = delete;
                JugDriver& operator=(const JugDriver&) = delete;
                // Explicitly allow moving
                //JugDriver(JugDriver&&) noexcept = default;
                //JugDriver& operator=(JugDriver&&) noexcept = default;

                // Lifetime Management (Always cause the driver to become busy!)
                // VFS is switched to "Safe Mode" when the function runs!
                bool run() ; // Returns true if the driver got triggered!

                // Adding projects/stary files
                // NOTE: These actions will be queued when the driver is busy!
                void processPaths(std::vector<std::string> &paths) ;
                void addProject(const std::string &path) ;
                void addStrayFile(const std::string &path) ;

                // Tools
                std::string lookupProject(const std::string &path, const std::string &previous = "") ; // Look up a parent project path

                // Queries
                bool busy() const noexcept { return this->isBusy; }
        };
    }
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
