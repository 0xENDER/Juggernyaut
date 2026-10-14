/**
 * @brief 
 * Driver Unit Configurations
**/

#pragma once

#include "common/headers.hpp"
#include "../dynamic.hpp" // JUG_DATA_API

namespace Data {
    namespace Driver {
        struct PackageConfigs {
            std::string name = "PACKAGE-NAME";
            std::string publisher = "PACKAGE-PUBLISHER";
            std::string version = "0.0.1";
            bool isDeprecated = false;
        };
        struct BuildFlags {
            bool ignoreWarnings = false;
            bool treatWarningsAsErrors = false;
        };
        struct BuildConfigs {
            BuildFlags flags;
            std::vector<std::string> inputPaths;
            std::string exportPath;
        };
        // Dependencies
        struct LocalDependency {
            std::string path;
        };
        struct OnlineDependency {
            std::string name = "";
            std::string publisher = "";
            std::string version = "";
        };
        // 'UnitConfigs' is derived from the 'jug.toml' configuration files!
        struct UnitConfigs {
            PackageConfigs package;
            BuildConfigs build;
            std::vector<LocalDependency> localDeps;
            std::vector<OnlineDependency> onlineDeps;
        };
    }
}
