/**
 * @brief 
 * Front-End Driver
**/

#include "JugDriver.hpp"

namespace Driver {
    namespace JugDriver {
        // Entry points lookup
        void entryCheck(DriverHooks &driverHooks, Data::Host::VFS *vfs, const std::string &uri, std::vector<std::string> &dirs) {
            if (!(vfs->exists(uri))) {
                // Couldn't find the specified uri
                if (driverHooks.onEntryError != nullptr) {
                    Diagnostic err;
                    err.severity = Diagnostics::Severity::Error;
                    err.message = CODE_700001(uri);
                    err.code = 700001;

                    driverHooks.onEntryError(std::move(err));
                }
            } else if (vfs->isDirectory(uri)) {
                const std::string configUri = vfs->join(uri, "jug.toml");
                if (vfs->exists(configUri)) {
                    dirs.push_back(std::move(configUri));
                } else if (driverHooks.onEntryError != nullptr) { // Couldn't find the specified uri
                    Diagnostic err;
                    err.severity = Diagnostics::Severity::Error;
                    err.message = CODE_700001(configUri);
                    err.code = 700001;

                    driverHooks.onEntryError(std::move(err));
                }
            } else {
                if (vfs->exists(uri)) {
                    if (vfs->filename(uri) == "jug.toml" || vfs->endsWith(uri, ".jug")) {
                        dirs.push_back(std::move(uri));
                    } else if (driverHooks.onEntryError != nullptr) { // Couldn't find the specified uri
                        Diagnostic err;
                        err.severity = Diagnostics::Severity::Error;
                        err.message = CODE_700001(uri);
                        err.code = 700001;

                        driverHooks.onEntryError(std::move(err));
                    }
                } else if (driverHooks.onEntryError != nullptr) { // Couldn't find the specified uri
                    Diagnostic err;
                    err.severity = Diagnostics::Severity::Error;
                    err.message = CODE_700001(uri);
                    err.code = 700001;

                    driverHooks.onEntryError(std::move(err));
                }
            }
        }
        void entryLookup(DriverHooks &driverHooks, Data::Host::VFS *vfs, const std::vector<std::string> &input, std::vector<std::string> &dirs) {
            // Notice that 'input' can include jug.toml files, *.jug files, or a directory path!
            for (auto uri : input) {
                entryCheck(driverHooks, vfs, uri, dirs);
            }
        }

        // Generate a dependency tree for an entry!
        void dependenciesLookup(Data::Host::VFS *vfs) {
            // ...
            (void)vfs;
        }

        void JugDriver::run() {
            // Update current driver hooks
            DriverHooks &driverHooks = this->configs.hooks.driver;

            Data::Host::VFS *vfs = (this->host).vfs;
            const std::vector<std::string> &input = this->configs.input;
            std::vector<std::string> unitDirectories;

            // Populate 'unitDirectories' with final entry points!
            entryLookup(driverHooks, vfs, input, unitDirectories);

            // Right now, 'unitDirectories' is a bit messy!
            // We need to check for files that may be under the same project,
            // and recheck project relations!
            // ...
        }
    }
}
