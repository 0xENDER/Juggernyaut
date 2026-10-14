/**
 * @brief
 * Handle .toml configs
**/

#include "configs.hpp"

// toml++
#include "tomlpp.hpp"

namespace Driver {
    namespace Configs {
        void emitDiagnostic(DriverHooks &driverHooks, const toml::source_region& src, Diagnostics::Severity severity, const std::string &message, int code) {
            if (driverHooks.onEntryError != nullptr) {
                Diagnostic diag;
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
        void processPackageSection(DriverHooks &driverHooks, const toml::table& tbl, UnitPackageConfigs &packageConfigs) {
            for (const auto& [key, val] : tbl) {
                std::string_view k = key.str();
                if (k == "name") {
                    if (auto v = val.value<std::string>()) {
                        packageConfigs.name = *v;
                        if (!isValidDepName(packageConfigs.name)) {
                            packageConfigs.name = "INVALID_PACKAGE_NAME";

                            emitDiagnostic(driverHooks, val.source(), Diagnostics::Severity::Error, CODE_700010, 700010);
                        }
                    } else {
                        emitDiagnostic(driverHooks, val.source(), Diagnostics::Severity::Error, CODE_700005("expected 'name' to be a string."), 700005);
                    }
                } else if (k == "publisher") {
                    if (auto v = val.value<std::string>()) {
                        packageConfigs.publisher = *v;
                        if (!isValidPublisherName(packageConfigs.publisher)) {
                            packageConfigs.publisher = "";

                            emitDiagnostic(driverHooks, val.source(), Diagnostics::Severity::Error, CODE_700011, 700011);
                        }
                    } else {
                        emitDiagnostic(driverHooks, val.source(), Diagnostics::Severity::Error, CODE_700005("expected 'publisher' to be a string."), 700005);
                    }
                } else if (k == "version") {
                    if (auto v = val.value<std::string>()) {
                        packageConfigs.version = *v;
                        if (!isValidStrictVersion(packageConfigs.version)) {
                            packageConfigs.version = "0.0.1";

                            emitDiagnostic(driverHooks, val.source(), Diagnostics::Severity::Error, CODE_700012, 700012);
                        }
                    } else {
                        emitDiagnostic(driverHooks, val.source(), Diagnostics::Severity::Error, CODE_700005("expected 'version' to be a string."), 700005);
                    }
                } else if (k == "deprecated") {
                    if (auto v = val.value<bool>()) {
                        // Read pkg.deprecated = *v;
                        packageConfigs.isDeprecated = *v;
                    } else {
                        emitDiagnostic(driverHooks, val.source(), Diagnostics::Severity::Error, CODE_700005("expected 'deprecated' to be a boolean."), 700005);
                    }
                } else {
                    // UNKNOWN VARIABLE IN [package]
                    emitDiagnostic(driverHooks, val.source(), Diagnostics::Severity::Warning, CODE_700006(std::string(k), "[package]"), 700006);
                }
            }
        }

        // [build]
        void processBuildSection(DriverHooks &driverHooks, const toml::table& tbl, UnitBuildConfigs &buildConfigs) {
            for (const auto& [key, val] : tbl) {
                std::string_view k = key.str();
                if (k == "export") {
                    if (auto v = val.value<std::string>()) {
                        buildConfigs.exportPath = *v;
                        if (!isValidSourcePath(buildConfigs.exportPath)) {
                            buildConfigs.exportPath = "";

                            emitDiagnostic(driverHooks, val.source(), Diagnostics::Severity::Error, CODE_700017, 700017);
                        }
                    } else {
                        emitDiagnostic(driverHooks, val.source(), Diagnostics::Severity::Error, CODE_700005("expected 'export' to be a string."), 700005);
                    }
                } else if (k == "input") {
                    if (auto str = val.value<std::string>()) {
                        if (isValidSourcePath(*str)) {
                            buildConfigs.inputPaths.push_back(*str);
                        } else {
                            emitDiagnostic(driverHooks, val.source(), Diagnostics::Severity::Error, CODE_700017, 700017);
                        }
                    } else if (const auto* arr = val.as_array()) {
                        for (const auto& elem : *arr) {
                            if (auto elemStr = elem.value<std::string>()) {
                                if (isValidSourcePath(*elemStr)) {
                                    buildConfigs.inputPaths.push_back(*elemStr);
                                } else {
                                    emitDiagnostic(driverHooks, elem.source(), Diagnostics::Severity::Error, CODE_700017, 700017);
                                }
                            } else {
                                emitDiagnostic(driverHooks, elem.source(), Diagnostics::Severity::Error, CODE_700005("expected all 'input' array elements to be strings."), 700005);
                            }
                        }
                    } else {
                        emitDiagnostic(driverHooks, val.source(), Diagnostics::Severity::Error, CODE_700005("expected 'input' to be either a string, or an array of strings."), 700005);
                    }
                } else if (k == "flags") {
                    if (const auto* flagsTbl = val.as_table()) {
                        static const std::unordered_map<std::string_view, bool UnitBuildFlags::*> flagsMapping = {
                            { "ignore-warnings", &UnitBuildFlags::ignoreWarnings },
                            { "treat-warnings-as-errors", &UnitBuildFlags::treatWarningsAsErrors }
                        };
                        for (const auto& [fKey, fVal] : *flagsTbl) {
                            std::string_view fk = fKey.str();
                            auto it = flagsMapping.find(fk);
                            if (it == flagsMapping.end()) {
                                emitDiagnostic(driverHooks, fVal.source(), Diagnostics::Severity::Warning, CODE_700006(std::string(fk), "[build.flags]"), 700006);
                                continue;
                            }
                            if (auto b = fVal.value<bool>()) {
                                buildConfigs.flags.*(it->second) = *b; // Carry over the value
                            } else {
                                emitDiagnostic(driverHooks, fVal.source(), Diagnostics::Severity::Error, CODE_700005("expected '" + std::string(fk) + "' to be a boolean."), 700005);
                            }
                        }
                    } else {
                        emitDiagnostic(driverHooks, val.source(), Diagnostics::Severity::Error, CODE_700005("expected 'flags' to be a table."), 700005);
                    }
                } else {
                    // UNKNOWN VARIABLE IN [build]
                    emitDiagnostic(driverHooks, val.source(), Diagnostics::Severity::Warning, CODE_700006(std::string(k), "[build]"), 700006);
                }
            }
        }

        // [dependencies]
        void processDependenciesSection(DriverHooks &driverHooks, const toml::table& tbl, std::vector<UnitLocalDependency> &localDeps, std::vector<UnitOnlineDependency> &onlineDeps) {
            for (const auto& [pkgKey, val] : tbl) {
                const std::string &name = std::string(pkgKey.str());
                if (auto ver = val.value<std::string>()) {
                    UnitOnlineDependency dep;
                    dep.name = name;
                    dep.version = *ver;
                    bool isValid = true;

                    if (!isValidDepName(dep.name)) {
                        isValid = false;

                        emitDiagnostic(driverHooks, val.source(), Diagnostics::Severity::Error, CODE_700010, 700010);
                    }
                    if (!isValidDepVersion(dep.version)) {
                        isValid = false;

                        emitDiagnostic(driverHooks, val.source(), Diagnostics::Severity::Error, CODE_700013, 700013);
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

                        emitDiagnostic(driverHooks, val.source(), Diagnostics::Severity::Error, CODE_700010, 700010);
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

                                emitDiagnostic(driverHooks, propVal.source(), Diagnostics::Severity::Error, CODE_700013, 700013);
                            }
                        } else if (prop == "publisher") {
                            publisher = str;
                            isOnlineProbSet = true;

                            if (!isValidPublisherName(publisher)) {
                                isValid = false;

                                emitDiagnostic(driverHooks, propVal.source(), Diagnostics::Severity::Error, CODE_700011, 700011);
                            }
                        } else if (prop == "path") {
                            path = str;
                            isPathSet = true;
                            
                            if (!isValidProjectPath(path)) {
                                isValid = false;

                                emitDiagnostic(driverHooks, propVal.source(), Diagnostics::Severity::Error, CODE_700016, 700016);
                            }
                        } else {
                            emitDiagnostic(driverHooks, propVal.source(), Diagnostics::Severity::Warning, CODE_700006(std::string(prop), name), 700006);
                        }
                    }

                    if (!isValid) {
                        continue;
                    }

                    // And we need to make sure that "path" is not used within online projects!
                    if (isPathSet && isOnlineProbSet) {
                        emitDiagnostic(driverHooks, val.source(), Diagnostics::Severity::Error, CODE_700014, 700014);
                    } else if (isPathSet) {
                        UnitLocalDependency dep;
                        dep.name = name;
                        dep.path = std::move(path);

                        localDeps.push_back(std::move(dep));
                    } else if (!isVersionSet) {
                        emitDiagnostic(driverHooks, val.source(), Diagnostics::Severity::Error, CODE_700015, 700015);
                    } else {
                        UnitOnlineDependency dep;
                        dep.name = name;
                        dep.version = std::move(version);
                        dep.publisher = std::move(publisher);

                        onlineDeps.push_back(std::move(dep));
                    }
                } else {
                    emitDiagnostic(driverHooks, val.source(), Diagnostics::Severity::Error, CODE_700005("expected '" + name + "' to be either a string, or a table."), 700005);
                }
            }
        }

        UnitConfigs fetchConfigs(Data::Host::VFS *vfs, DriverHooks &driverHooks, const std::string &path) {
            UnitConfigs configs;

            // Process TOML data
            const std::string &rawContent = vfs->readFile(path);
            toml::parse_result result = toml::parse(rawContent);
            if (!result) {
                const toml::parse_error& err = result.error();
                std::string description = std::string(err.description());

                emitDiagnostic(driverHooks, err.source(), Diagnostics::Severity::Error, CODE_700007(description), 700007);

                return configs;
            }

            toml::table table = std::move(result).table();

            // Iterate over root-level entries
            for (const auto& [key, val] : table) {
                std::string_view sectionName = key.str();
                if (sectionName == "package") {
                    if (const auto* sub_tbl = val.as_table()) {
                        processPackageSection(driverHooks, *sub_tbl, configs.package);
                    } else {
                        emitDiagnostic(driverHooks, val.source(), Diagnostics::Severity::Error, CODE_700005("expected 'package' to be a section table."), 700005);
                    }
                } else if (sectionName == "build") {
                    if (const auto* sub_tbl = val.as_table()) {
                        processBuildSection(driverHooks, *sub_tbl, configs.build);
                    } else {
                        emitDiagnostic(driverHooks, val.source(), Diagnostics::Severity::Error, CODE_700005("expected 'build' to be a section table."), 700005);
                    }
                } else if (sectionName == "dependencies") {
                    if (const auto* sub_tbl = val.as_table()) {
                        processDependenciesSection(driverHooks, *sub_tbl, configs.localDeps, configs.onlineDeps);
                    } else {
                        emitDiagnostic(driverHooks, val.source(), Diagnostics::Severity::Error, CODE_700005("expected 'dependencies' to be a section table."), 700005);
                    }
                } else {
                    // UNKNOWN TOP-LEVEL SECTION OR VARIABLE
                    emitDiagnostic(driverHooks, val.source(), Diagnostics::Severity::Warning, CODE_700008(std::string(sectionName)), 700008);
                }
            }

            return configs;
        }
    }
}
