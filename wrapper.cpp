/**
 * @brief
 * Jug command wrapper
**/

// Basic C++
#include <iostream>
#include <string>
#include <filesystem>
#include <cstdlib>

// Cross-platform process spawning helper
int spawn (const std::string& binary_name, int argc, char* argv[]) {
    std::filesystem::path exe_dir = std::filesystem::path(argv[0]).parent_path();
    std::filesystem::path target_exe = exe_dir / binary_name;

    #ifdef _WIN32
        target_exe.replace_extension(".exe");
    #endif

    if (!std::filesystem::exists(target_exe)) {
        std::cerr << "Error: Subcommand binary not found at " << target_exe << "\n";
        return 1;
    }

    // Reconstruct the command line forwarding argv[2] onwards
    std::string command = "\"" + target_exe.string() + "\"";
    for (int i = 2; i < argc; ++i) {
        command += " ";
        command += argv[i];
    }

    // Native execution call
    return std::system(command.c_str());
}

int main (int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Juggernyaut Toolchain\n\n"
                  << "Usage: jug [command] [options]\n\n"
                  << "Commands:\n"
                  << "    cmp    Launch the compiler\n"
                  << "    pck    Launch the package manager\n"
                  << "    lsp    Launch the language server\n";
        return 0;
    }

    std::string subcommand = argv[1];

    if (subcommand == "cmp") return spawn("JuggernyautCompiler", argc, argv);
    if (subcommand == "pck") return spawn("JuggernyautPackageManager", argc, argv);
    if (subcommand == "lsp") return spawn("JuggernyautServer", argc, argv);

    std::cerr << "Error: Unknown command '" << subcommand << "'\n";
    return 1;
}
