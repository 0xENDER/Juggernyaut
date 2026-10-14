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

        void processPackageSection(DriverHooks &driverHooks, const toml::table& tbl, UnitPackageConfigs &packageConfigs) {
            for (const auto& [key, val] : tbl) {
                std::string_view k = key.str();
                if (k == "name") {
                    if (auto v = val.value<std::string>()) {
                        packageConfigs.name = *v;
                    } else {
                        emitDiagnostic(driverHooks, val.source(), Diagnostics::Severity::Error, CODE_700005("expected 'name' to be a string."), 700005);
                    }
                } else if (k == "publisher") {
                    if (auto v = val.value<std::string>()) {
                        packageConfigs.publisher = *v;
                    } else {
                        emitDiagnostic(driverHooks, val.source(), Diagnostics::Severity::Error, CODE_700005("expected 'publisher' to be a string."), 700005);
                    }
                } else if (k == "version") {
                    if (auto v = val.value<std::string>()) {
                        packageConfigs.version = *v;
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

        void processBuildSection(DriverHooks &driverHooks, const toml::table& tbl, UnitBuildConfigs &buildConfigs) {
            for (const auto& [key, val] : tbl) {
                std::string_view k = key.str();
                if (k == "export") {
                    if (auto v = val.value<std::string>()) {
                        buildConfigs.exportPath = *v;
                    } else {
                        emitDiagnostic(driverHooks, val.source(), Diagnostics::Severity::Error, CODE_700005("expected 'export' to be a string."), 700005);
                    }
                } else if (k == "input") {
                    if (auto str = val.value<std::string>()) {
                        buildConfigs.inputPaths.push_back(*str);
                    } else if (const auto* arr = val.as_array()) {
                        for (const auto& elem : *arr) {
                            if (auto elem_str = elem.value<std::string>()) {
                                buildConfigs.inputPaths.push_back(*elem_str);
                            } else {
                                emitDiagnostic(driverHooks, val.source(), Diagnostics::Severity::Error, CODE_700005("expected all 'input' array elements to be strings."), 700005);
                            }
                        }
                    } else {
                        emitDiagnostic(driverHooks, val.source(), Diagnostics::Severity::Error, CODE_700005("expected 'input' to be either a string, or an array of strings."), 700005);
                    }
                } else if (k == "flags") {
                    static const std::unordered_map<std::string_view, bool UnitBuildFlags::*> flagsMapping = {
                        { "ignore-warnings", &UnitBuildFlags::ignoreWarnings },
                        { "treat-warnings-as-errors", &UnitBuildFlags::treatWarningsAsErrors }
                    };
                    for (const auto& [fKey, fVal] : tbl) {
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
                    // UNKNOWN VARIABLE IN [build]
                    emitDiagnostic(driverHooks, val.source(), Diagnostics::Severity::Warning, CODE_700006(std::string(k), "[build]"), 700006);
                }
            }
        }

        void processDependenciesSection(DriverHooks &driverHooks, const toml::table& tbl, std::vector<UnitLocalDependency> &localDeps, std::vector<UnitOnlineDependency> &onlineDeps) {
            // ...
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
                } /*else if (sectionName == "dependencies") {
                    if (const auto* sub_tbl = val.as_table()) {
                        processDependenciesSection(driverHooks, *sub_tbl, configs.localDeps, configs.onlineDeps);
                    } else {
                        emitDiagnostic(driverHooks, val.source(), Diagnostics::Severity::Error, CODE_700005("expected 'dependencies' to be a section table."), 700005);
                    }
                }*/ else {
                    // UNKNOWN TOP-LEVEL SECTION OR VARIABLE
                    emitDiagnostic(driverHooks, val.source(), Diagnostics::Severity::Warning, CODE_700008(std::string(sectionName)), 700008);
                }
            }

            return configs;
        }
    }
}
