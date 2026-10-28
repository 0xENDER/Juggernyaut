/**
 * @brief
 * Handle .toml configs
**/

#pragma once

#include "common/headers.hpp"
#include "dynamic.hpp" // JUG_DRIVER_API

// Host/System
#include "../data/host/VFS.hpp"

// Environment data
#include "./../data/driver/Environment.hpp"

#include "Hooks.hpp"

namespace Driver {
    namespace Environment {
        using Environment = Data::Driver::Environment;
        using EnvironmentId = Data::Driver::EnvironmentId;
        using PackageConfigs = Data::Driver::PackageConfigs;
        using BuildConfigs = Data::Driver::BuildConfigs;
        using BuildFlags = Data::Driver::BuildFlags;
        using LocalDependency = Data::Driver::LocalDependency;
        using OnlineDependency = Data::Driver::OnlineDependency;

        using File = Data::Host::File;
        using VFS = Data::Host::VFS;

        // Load configs from a 'jug.toml' file (Assuming the file is accessible!)
        Environment fetchEnvironment(VFS *vfs, DriverHooks &driverHooks, const std::string &path) ;
    }
}
