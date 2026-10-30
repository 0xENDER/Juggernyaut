/**
 * @brief 
 * Front-End Driver
**/

#include "JugDriver.hpp"

// 'jug.toml'-related
#include "environment.hpp"

namespace Driver {
    namespace JugDriver {
        // Internal
        void JugDriver::processPendingPaths() {
            // Drain any paths that were added whilst a previous run was busy
            if (!this->pendingPaths.empty()) {
                std::vector<std::string> queued = std::move(this->pendingPaths);
                this->pendingPaths.clear();
                this->processPaths(queued);
            }
        }

        // Lifetime Management (Internal)
        void JugDriver::refreshSetups() {
            // Reference needed objects
            DriverHooks &driverHooks = this->configs.hooks.driver;

            for (std::unique_ptr<Setup>& setup : std::views::values(this->setups)) {
                if (setup->file != nullptr && setup->file->version > setup->version) {
                    Environment::refreshEnvironment(setup.get(), driverHooks);
                }
            }
        }
        bool JugDriver::refreshDependencyGraph() {
            return false;
        }

        // Lifetime Management (Always cause the driver to become busy!)
        // VFS is switched to "Safe Mode" when the function runs!
        bool JugDriver::run() {
            if (this->isBusy) {
                return false;
            }

            // Check for pending path jobs
            this->processPendingPaths();

            // Enter VFS Safe Mode
            this->isBusy = true;
            this->host.vfs->setSafeMode(true);

            // Make sure jug.toml configs are up-to-date
            this->refreshSetups();

            // (Re)construct the dependency graph
            if(!(this->refreshDependencyGraph())) {
                return false;
            }

            // Initiate parallel independent session runs!
            // ...

            // Exit VFS Safe Mode
            this->isBusy = false;
            this->host.vfs->setSafeMode(false);

            return true;
        }

        // Adding projects/stary files
        void JugDriver::processPaths(std::vector<std::string> &paths) {
            // Reference needed objects
            Data::Host::VFS *vfs = (this->host).vfs;

            // Prevent changes during a run!
            if (this->isBusy) {
                this->pendingPaths.insert(this->pendingPaths.end(), paths.begin(), paths.end());
                return;
            }
            this->processPendingPaths();

            for (auto path: paths) {
                if (vfs->isDirectory(path) || vfs->filename(path) == "jug.toml") {
                    this->addProject(path);
                } else {
                    this->addStrayFile(path);
                }
            }
        }
        bool dirConfigLookup(DriverHooks &driverHooks, Data::Host::VFS *vfs, const std::string &uri, const std::string &sourceUri, std::string &outputUri) {
            const std::string configUri = vfs->join(uri, "jug.toml");
            const std::string parentDir = vfs->getParentDirectory(uri);

            if (vfs->exists(configUri)) {
                outputUri = std::move(configUri);
                return true;
            } else if (parentDir == uri) {
                if (driverHooks.onEntryError != nullptr) {
                    Diagnostic err;
                    err.severity = Diagnostics::Severity::Error;
                    err.message = CODE_700002(sourceUri);
                    err.code = 700002;

                    driverHooks.onEntryError(std::move(err));
                }
                return false;
            } else {
                return dirConfigLookup(driverHooks, vfs, parentDir, sourceUri, outputUri);
            }
        }
        void JugDriver::addProject(const std::string &path) {
            // Reference needed objects
            DriverHooks &driverHooks = this->configs.hooks.driver;
            Data::Host::VFS *vfs = (this->host).vfs;

            // Prevent changes during a run!
            if (this->isBusy) {
                this->pendingPaths.push_back(path);
                return;
            }
            this->processPendingPaths();

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
                Environment::Environment env = Environment::fetchEnvironment(vfs, driverHooks, path);
                (void)env;
            } else if (vfs->isDirectory(path)) {
                std::string configUri;
                if(dirConfigLookup(driverHooks, vfs, path, path, configUri)) {
                    // LOAD UP THE CONFIGS
                    Environment::Environment env = Environment::fetchEnvironment(vfs, driverHooks, configUri);
                    (void)env;
                } else {
                    // LOAD UP THE DEFAULT CONFIGS
                    Environment::Environment env;
                    (void)env;
                }
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

            // Prevent changes during a run!
            if (this->isBusy) {
                this->pendingPaths.push_back(path);
                return;
            }
            this->processPendingPaths();

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
                // ADD THE FILE!
                // MUST CHECK FOR CONFIGS FIRST!
            } else if (driverHooks.onEntryError != nullptr) {
                Diagnostic err;
                err.severity = Diagnostics::Severity::Error;
                err.message = CODE_700004(path);
                err.code = 700004;

                driverHooks.onEntryError(std::move(err));
            }
        }

        // Tools
        std::string JugDriver::lookupProject(const std::string &path, const std::string &previous) {
            if (path.empty() || path == previous) {
                return ""; // Reached filesystem root
            }
            Data::Host::VFS *vfs = this->host.vfs;

            if (vfs->filename(path) == "jug.toml") {
                if (vfs->exists(path)) {
                    return path;
                }
                const std::string dir = vfs->getParentDirectory(path);

                return this->lookupProject(vfs->getParentDirectory(dir), dir);
            } else if (vfs->isDirectory(path)) {
                const std::string newPath = vfs->join(path, "jug.toml");
                if (vfs->exists(newPath)) {
                    return newPath;
                }

                return this->lookupProject(vfs->getParentDirectory(path), path);
            } else {
                return this->lookupProject(vfs->getParentDirectory(path), path);
            }
        }
    }
}
