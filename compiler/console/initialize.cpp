/**
 * @brief
 * Handle CLI initialisation
**/

#include "initialize.hpp"

#include "../base/info.hpp"

#include "basic.hpp"
#include "optimization.hpp"

namespace Console {
    namespace Internal {
        // Track status
        static bool isInitialized = false;
        bool isTrackingTime = false;
        std::chrono::high_resolution_clock::time_point start;

        void startClock() {
            start = std::chrono::high_resolution_clock::now();
            isTrackingTime = true;
        }

        // Handle CLI initialisation
        void initialize() {
            if (isInitialized) { // TMP
                return;
            }

            // Optimise console output
            Optimization::optimize();

            /**
             *   ,-.       _,---._ __  / \
             *  /  )    .-'       `./ /   \   Juggernyaut v??? - <arch>
             * (  (   ,'            `/    /|  Build: <build_type> - <arch>
             *  \  `-"             \'\   / |  Platform: <os_name> <os_version>
             *   `.              ,  \ \ /  |
             *    /`.          ,'-`----Y   |  (C) 2026 Ender-ing GitHub Organisation
             *   (            ;        |   '
             *   |  ,-.    ,-'         |  /
             *   |  | (   |      jug   | /    (Cat-chy Art by Hayley Jane Wakenshaw)
             *   )  |  \  `.___________|/
             *   `--'   `--'
            **/
            // Print initial console prompt
            std::cout << std::endl << '\n' << color("  ,-.       _,---._ __ ", Color::golden_rod)
                << color(" / \\    Juggernyaut ", Color::light_sea_green) << color("v", Color::blue_violet)
                << color(Base::Info::version, Color::blue_violet) << '\n';
            std::cout << color(" /  )    .-'       `./", Color::golden_rod)
                << color(" /   \\   Build: ", Color::light_sea_green)
                << color(Base::Info::build, Color::light_sea_green)
                << color(" - ", Color::light_sea_green) << color(Base::Info::arch, Color::light_sea_green) << '\n';
            std::cout << color("(  (   ,'            `", Color::golden_rod)
                << color("/    /|  Platform: ", Color::light_sea_green)
                << color(Base::Info::osName, Color::light_sea_green) << color(" ", Color::light_sea_green)
                << color(Base::Info::osVersion, Color::light_sea_green) << '\n';
            std::cout << color(" \\  `-\"             \\'", Color::golden_rod)
                << color("\\   / |", Color::light_sea_green) << '\n';
            std::cout << color("  `.              ,  \\", Color::golden_rod)
                << color(" \\ /  |  (C) 2026 Ender-ing GitHub Organisation", Color::light_sea_green)
                << '\n';
            std::cout << color("   /`.          ,'", Color::golden_rod) << color("-", Color::light_sea_green)
                << color("`", Color::golden_rod) << color("----Y   |", Color::light_sea_green) << '\n';
            std::cout << color("  (            ;", Color::golden_rod)
                << color("        |   '", Color::light_sea_green) << '\n';
            std::cout << color("  |  ,-.    ,-'", Color::golden_rod) << color("         |  /", Color::light_sea_green)
                << '\n';
            std::cout << color("  |  | (   |", Color::golden_rod)
                << color("      jug   | /    (Cat-chy Art by Hayley Jane Wakenshaw)", Color::light_sea_green)
                << '\n';
            std::cout << color("  )  |  \\  `.", Color::golden_rod) << color("___________|/", Color::light_sea_green)
                << '\n';
            std::cout << color("  `--'   `--'", Color::golden_rod) << '\n' << std::endl;

            // Update status to prevent duplicate calls
            isInitialized = true;
        }
    }
}
