/**
 * @brief
 * Handle .toml configs
**/

#include "environment.hpp"

// toml++
#include "tomlpp.hpp"

namespace Driver {
    namespace Environment {
        void emitDiagnostic(DriverHooks &driverHooks, FileId fileId, const toml::source_region& src, Diagnostics::Severity severity, const std::string &message, int code) {
            if (driverHooks.onEntryError != nullptr) {
                Diagnostic diag;
                // Origin
                diag.origin = Diagnostics::Origin::File;
                diag.fileId = fileId;

                // Diagnositc info
                diag.severity = severity;
                diag.message = message;
                diag.code = code;

                // Source range
                diag.range.start.line = static_cast<uint32_t>((int) src.begin.line);
                diag.range.start.character = static_cast<uint32_t>((int) src.begin.column);
                diag.range.end.line = static_cast<uint32_t>((int) src.end.line);
                diag.range.end.character = static_cast<uint32_t>((int) src.end.column);

                // MISSING SOURCE ID INFO
                // MUST REWORK THE STORE CLASS FIRST!

                driverHooks.onEntryError(std::move(diag));
            }
        }

        // Data validation
        bool isValidDepName(const std::string &name) {
            return false;
        }
        bool isValidPublisherName(const std::string &name) {
            return false;
        }
        bool isValidStrictVersion(const std::string &ver) {
            return false;
        }
        bool isValidDepVersion(const std::string &ver) {
            return false;
        }
        bool isValidProjectPath(const std::string &path) {
            return false;
        }
        bool isValidSourcePath(const std::string &path) {
            return false;
        }

        // [Package]
        void processPackageSection(DriverHooks &driverHooks, FileId fileId, const toml::table& tbl, PackageConfigs &packageConfigs) {
            for (const auto& [key, val] : tbl) {
                std::string_view k = key.str();
                if (k == "name") {
                    if (auto v = val.value<std::string>()) {
                        packageConfigs.name = *v;
                        if (!isValidDepName(packageConfigs.name)) {
                            packageConfigs.name = "INVALID_PACKAGE_NAME";

                            emitDiagnostic(driverHooks, fileId, val.source(), Diagnostics::Severity::Error, CODE_700010, 700010);
                        }
                    } else {
                        emitDiagnostic(driverHooks, fileId, val.source(), Diagnostics::Severity::Error, CODE_700005("expected 'name' to be a string."), 700005);
                    }
                } else if (k == "publisher") {
                    if (auto v = val.value<std::string>()) {
                        packageConfigs.publisher = *v;
                        if (!isValidPublisherName(packageConfigs.publisher)) {
                            packageConfigs.publisher = "";

                            emitDiagnostic(driverHooks, fileId, val.source(), Diagnostics::Severity::Error, CODE_700011, 700011);
                        }
                    } else {
                        emitDiagnostic(driverHooks, fileId, val.source(), Diagnostics::Severity::Error, CODE_700005("expected 'publisher' to be a string."), 700005);
                    }
                } else if (k == "version") {
                    if (auto v = val.value<std::string>()) {
                        packageConfigs.version = *v;
                        if (!isValidStrictVersion(packageConfigs.version)) {
                            packageConfigs.version = "0.0.1";

                            emitDiagnostic(driverHooks, fileId, val.source(), Diagnostics::Severity::Error, CODE_700012, 700012);
                        }
                    } else {
                        emitDiagnostic(driverHooks, fileId, val.source(), Diagnostics::Severity::Error, CODE_700005("expected 'version' to be a string."), 700005);
                    }
                } else if (k == "deprecated") {
                    if (auto v = val.value<bool>()) {
                        // Read pkg.deprecated = *v;
                        packageConfigs.isDeprecated = *v;
                    } else {
                        emitDiagnostic(driverHooks, fileId, val.source(), Diagnostics::Severity::Error, CODE_700005("expected 'deprecated' to be a boolean."), 700005);
                    }
                } else {
                    // UNKNOWN VARIABLE IN [package]
                    emitDiagnostic(driverHooks, fileId, val.source(), Diagnostics::Severity::Warning, CODE_700006(std::string(k), "[package]"), 700006);
                }
            }
        }

        // [build]
        void processBuildSection(DriverHooks &driverHooks, FileId fileId, const toml::table& tbl, BuildConfigs &buildConfigs) {
            for (const auto& [key, val] : tbl) {
                std::string_view k = key.str();
                if (k == "export") {
                    if (auto v = val.value<std::string>()) {
                        buildConfigs.exportPath = *v;
                        if (!isValidSourcePath(buildConfigs.exportPath)) {
                            buildConfigs.exportPath = "";

                            emitDiagnostic(driverHooks, fileId, val.source(), Diagnostics::Severity::Error, CODE_700017, 700017);
                        }
                    } else {
                        emitDiagnostic(driverHooks, fileId, val.source(), Diagnostics::Severity::Error, CODE_700005("expected 'export' to be a string."), 700005);
                    }
                } else if (k == "input") {
                    if (auto str = val.value<std::string>()) {
                        if (isValidSourcePath(*str)) {
                            buildConfigs.inputPaths.push_back(*str);
                        } else {
                            emitDiagnostic(driverHooks, fileId, val.source(), Diagnostics::Severity::Error, CODE_700017, 700017);
                        }
                    } else if (const auto* arr = val.as_array()) {
                        for (const auto& elem : *arr) {
                            if (auto elemStr = elem.value<std::string>()) {
                                if (isValidSourcePath(*elemStr)) {
                                    buildConfigs.inputPaths.push_back(*elemStr);
                                } else {
                                    emitDiagnostic(driverHooks, fileId, elem.source(), Diagnostics::Severity::Error, CODE_700017, 700017);
                                }
                            } else {
                                emitDiagnostic(driverHooks, fileId, elem.source(), Diagnostics::Severity::Error, CODE_700005("expected all 'input' array elements to be strings."), 700005);
                            }
                        }
                    } else {
                        emitDiagnostic(driverHooks, fileId, val.source(), Diagnostics::Severity::Error, CODE_700005("expected 'input' to be either a string, or an array of strings."), 700005);
                    }
                } else if (k == "flags") {
                    if (const auto* flagsTbl = val.as_table()) {
                        static const std::unordered_map<std::string_view, bool BuildFlags::*> flagsMapping = {
                            { "ignore-warnings", &BuildFlags::ignoreWarnings },
                            { "treat-warnings-as-errors", &BuildFlags::treatWarningsAsErrors }
                        };
                        for (const auto& [fKey, fVal] : *flagsTbl) {
                            std::string_view fk = fKey.str();
                            auto it = flagsMapping.find(fk);
                            if (it == flagsMapping.end()) {
                                emitDiagnostic(driverHooks, fileId, fVal.source(), Diagnostics::Severity::Warning, CODE_700006(std::string(fk), "[build.flags]"), 700006);
                                continue;
                            }
                            if (auto b = fVal.value<bool>()) {
                                buildConfigs.flags.*(it->second) = *b; // Carry over the value
                            } else {
                                emitDiagnostic(driverHooks, fileId, fVal.source(), Diagnostics::Severity::Error, CODE_700005("expected '" + std::string(fk) + "' to be a boolean."), 700005);
                            }
                        }
                    } else {
                        emitDiagnostic(driverHooks, fileId, val.source(), Diagnostics::Severity::Error, CODE_700005("expected 'flags' to be a table."), 700005);
                    }
                } else {
                    // UNKNOWN VARIABLE IN [build]
                    emitDiagnostic(driverHooks, fileId, val.source(), Diagnostics::Severity::Warning, CODE_700006(std::string(k), "[build]"), 700006);
                }
            }
        }

        // [dependencies]
        void processDependenciesSection(DriverHooks &driverHooks, FileId fileId, const toml::table& tbl, std::vector<LocalDependency> &localDeps, std::vector<OnlineDependency> &onlineDeps) {
            for (const auto& [pkgKey, val] : tbl) {
                const std::string &name = std::string(pkgKey.str());
                if (auto ver = val.value<std::string>()) {
                    OnlineDependency dep;
                    dep.name = name;
                    dep.version = *ver;
                    bool isValid = true;

                    if (!isValidDepName(dep.name)) {
                        isValid = false;

                        emitDiagnostic(driverHooks, fileId, val.source(), Diagnostics::Severity::Error, CODE_700010, 700010);
                    }
                    if (!isValidDepVersion(dep.version)) {
                        isValid = false;

                        emitDiagnostic(driverHooks, fileId, val.source(), Diagnostics::Severity::Error, CODE_700013, 700013);
                    }

                    if (isValid) {
                        onlineDeps.push_back(std::move(dep));
                    }
                } else if (const auto* depTable = val.as_table()) {
                    std::string version = "",
                        publisher = "",
                        path = "";
                    bool isValid = true;
                    bool isPathSet = false,
                        isVersionSet = false,
                        isOnlineProbSet = false;

                    if (!isValidDepName(name)) {
                        isValid = false;

                        emitDiagnostic(driverHooks, fileId, val.source(), Diagnostics::Severity::Error, CODE_700010, 700010);
                    }

                    for (const auto& [propKey, propVal] : *depTable) {
                        std::string_view prop = propKey.str();
                        auto str = propVal.value<std::string>().value_or("");
                        if (prop == "version") {
                            version = str;
                            isVersionSet = true;
                            isOnlineProbSet = true;

                            if (!isValidDepVersion(version)) {
                                isValid = false;

                                emitDiagnostic(driverHooks, fileId, propVal.source(), Diagnostics::Severity::Error, CODE_700013, 700013);
                            }
                        } else if (prop == "publisher") {
                            publisher = str;
                            isOnlineProbSet = true;

                            if (!isValidPublisherName(publisher)) {
                                isValid = false;

                                emitDiagnostic(driverHooks, fileId, propVal.source(), Diagnostics::Severity::Error, CODE_700011, 700011);
                            }
                        } else if (prop == "path") {
                            path = str;
                            isPathSet = true;
                            
                            if (!isValidProjectPath(path)) {
                                isValid = false;

                                emitDiagnostic(driverHooks, fileId, propVal.source(), Diagnostics::Severity::Error, CODE_700016, 700016);
                            }
                        } else {
                            emitDiagnostic(driverHooks, fileId, propVal.source(), Diagnostics::Severity::Warning, CODE_700006(std::string(prop), name), 700006);
                        }
                    }

                    if (!isValid) {
                        continue;
                    }

                    // And we need to make sure that "path" is not used within online projects!
                    if (isPathSet && isOnlineProbSet) {
                        emitDiagnostic(driverHooks, fileId, val.source(), Diagnostics::Severity::Error, CODE_700014, 700014);
                    } else if (isPathSet) {
                        LocalDependency dep;
                        dep.name = name;
                        dep.path = std::move(path);

                        localDeps.push_back(std::move(dep));
                    } else if (!isVersionSet) {
                        emitDiagnostic(driverHooks, fileId, val.source(), Diagnostics::Severity::Error, CODE_700015, 700015);
                    } else {
                        OnlineDependency dep;
                        dep.name = name;
                        dep.version = std::move(version);
                        dep.publisher = std::move(publisher);

                        onlineDeps.push_back(std::move(dep));
                    }
                } else {
                    emitDiagnostic(driverHooks, fileId, val.source(), Diagnostics::Severity::Error, CODE_700005("expected '" + name + "' to be either a string, or a table."), 700005);
                }
            }
        }

        void refreshEnvironment(Environment* env, DriverHooks& driverHooks) {
            if (env == nullptr) {
                return;
            }
            const File*& file = env->file;
            if (file == nullptr) {
                return;
            }
            env->package = {};
            env->build = {};
            env->localDeps.clear();
            env->onlineDeps.clear();

            // Process TOML data
            toml::parse_result result = toml::parse(file->content);
            if (!result) {
                const toml::parse_error& err = result.error();
                std::string description = std::string(err.description());

                emitDiagnostic(driverHooks, file->id, err.source(), Diagnostics::Severity::Error, CODE_700007(description), 700007);

                return;
            }

            toml::table table = std::move(result).table();

            // Iterate over root-level entries
            for (const auto& [key, val] : table) {
                std::string_view sectionName = key.str();
                if (sectionName == "package") {
                    if (const auto* sub_tbl = val.as_table()) {
                        processPackageSection(driverHooks, file->id, *sub_tbl, env->package);
                    } else {
                        emitDiagnostic(driverHooks, file->id, val.source(), Diagnostics::Severity::Error, CODE_700005("expected 'package' to be a section table."), 700005);
                    }
                } else if (sectionName == "build") {
                    if (const auto* sub_tbl = val.as_table()) {
                        processBuildSection(driverHooks, file->id, *sub_tbl, env->build);
                    } else {
                        emitDiagnostic(driverHooks, file->id, val.source(), Diagnostics::Severity::Error, CODE_700005("expected 'build' to be a section table."), 700005);
                    }
                } else if (sectionName == "dependencies") {
                    if (const auto* sub_tbl = val.as_table()) {
                        processDependenciesSection(driverHooks, file->id, *sub_tbl, env->localDeps, env->onlineDeps);
                    } else {
                        emitDiagnostic(driverHooks, file->id, val.source(), Diagnostics::Severity::Error, CODE_700005("expected 'dependencies' to be a section table."), 700005);
                    }
                } else {
                    // UNKNOWN TOP-LEVEL SECTION OR VARIABLE
                    emitDiagnostic(driverHooks, file->id, val.source(), Diagnostics::Severity::Warning, CODE_700008(std::string(sectionName)), 700008);
                }
            }

            // Update environment version
            env->version = env->file->version;
        }
        Environment fetchEnvironment(VFS *vfs, DriverHooks &driverHooks, const std::string &path) {
            Environment env;
            if (vfs == nullptr) {
                return env;
            }
            env.file = vfs->readFile(path);

            // Populate the environment
            refreshEnvironment(&env, driverHooks);

            return env;
        }
    }
}
