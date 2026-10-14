/**
 * @brief
 * Handle .toml configs
**/

#pragma once

#include "common/headers.hpp"
#include "dynamic.hpp" // JUG_DRIVER_API

// Host/System
#include "../data/host/VFS.hpp"

// Configs
#include "./../data/driver/UnitConfigs.hpp"

#include "Hooks.hpp"

namespace Driver {
    namespace Configs {
        using UnitConfigs = Data::Driver::UnitConfigs;
        using UnitPackageConfigs = Data::Driver::PackageConfigs;
        using UnitBuildConfigs = Data::Driver::BuildConfigs;
        using UnitBuildFlags = Data::Driver::BuildFlags;
        using UnitLocalDependency = Data::Driver::LocalDependency;
        using UnitOnlineDependency = Data::Driver::OnlineDependency;

        // Load configs from a 'jug.toml' file
        UnitConfigs fetchConfigs(Data::Host::VFS *vfs, DriverHooks &driverHooks, const std::string &path) ;
    }
}
