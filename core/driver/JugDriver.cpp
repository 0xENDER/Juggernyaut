/**
 * @brief 
 * Front-End Driver
**/

#include "JugDriver.hpp"

namespace Driver {
    namespace JugDriver {
        // Generate a dependency tree for an entry!
        void dependenciesLookup(Data::Host::VFS *vfs) {
            // ...
            (void)vfs;
        }

        void JugDriver::run() {
            // ...
        }

        void JugDriver::processPaths(std::vector<std::string> &paths) {
            // Reference needed objects
            Data::Host::VFS *vfs = (this->host).vfs;

            for (auto path: paths) {
                if (vfs->isDirectory(path) || vfs->filename(path) == "jug.toml") {
                    this->addProject(path);
                } else {
                    this->addStrayFile(path);
                }
            }
        }

        void dirConfigLookup(DriverHooks &driverHooks, Data::Host::VFS *vfs, const std::string &uri, const std::string &sourceUri) {
            const std::string configUri = vfs->join(uri, "jug.toml");
            const std::string parentDir = vfs->getParentDirectory(uri);

            if (vfs->exists(configUri)) {
                // LOAD UP THE CONFIGS
            } else if (parentDir == uri) {
                if (driverHooks.onEntryError != nullptr) {
                    Diagnostic err;
                    err.severity = Diagnostics::Severity::Error;
                    err.message = CODE_700002(sourceUri);
                    err.code = 700002;

                    driverHooks.onEntryError(std::move(err));
                }
            } else {
                dirConfigLookup(driverHooks, vfs, parentDir, sourceUri);
            }
        }
        void JugDriver::addProject(const std::string &path) {
            // Reference needed objects
            DriverHooks &driverHooks = this->configs.hooks.driver;
            Data::Host::VFS *vfs = (this->host).vfs;

            if (!(vfs->exists(path))) {
                // Couldn't find the specified uri
                if (driverHooks.onEntryError != nullptr) {
                    Diagnostic err;
                    err.severity = Diagnostics::Severity::Error;
                    err.message = CODE_700001(path);
                    err.code = 700001;

                    driverHooks.onEntryError(std::move(err));
                }
            } else if (vfs->filename(path) == "jug.toml") {
                // LOAD UP THE CONFIGS
            } else if (vfs->isDirectory(path)) {
                dirConfigLookup(driverHooks, vfs, path, path);
            } else if (driverHooks.onEntryError != nullptr) {
                Diagnostic err;
                err.severity = Diagnostics::Severity::Error;
                err.message = CODE_700003(path);
                err.code = 700003;

                driverHooks.onEntryError(std::move(err));
            }
        }

        void JugDriver::addStrayFile(const std::string &path) {
            // Reference needed objects
            DriverHooks &driverHooks = this->configs.hooks.driver;
            Data::Host::VFS *vfs = (this->host).vfs;

            if (!(vfs->exists(path))) {
                // Couldn't find the specified uri
                if (driverHooks.onEntryError != nullptr) {
                    Diagnostic err;
                    err.severity = Diagnostics::Severity::Error;
                    err.message = CODE_700001(path);
                    err.code = 700001;

                    driverHooks.onEntryError(std::move(err));
                }
            } else if (vfs->endsWith(path, ".jug")) {
                // TRY TO CHECK UNDER WHICH PSUDO-PROJECT THIS FILE FALLS!
                // IF IT'S ALREADY PART OF A PROJECT, STOP THIS!
            } else if (driverHooks.onEntryError != nullptr) {
                Diagnostic err;
                err.severity = Diagnostics::Severity::Error;
                err.message = CODE_700004(path);
                err.code = 700004;

                driverHooks.onEntryError(std::move(err));
            }
        }
    }
}
