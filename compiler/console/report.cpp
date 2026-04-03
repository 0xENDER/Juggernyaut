/**
 * @brief
 * Handle CLI report displays
**/

#include "report.hpp"

// Comms headers
#include "console.hpp"
#include "basic.hpp"

// Shortent the syntax for printing to the console
#define INTERNAL_C_OUT(DATA, CHANNEL_VAR)                   \
    if (CHANNEL_VAR == 0) {                                 \
        std::cout << DATA << std::flush;                    \
    } else {                                                \
        std::cerr << DATA << std::flush;                    \
    }                                                       \

namespace Console {
    namespace Internal {
        // Handle reports CLI outputs
        namespace Reports {
            // Sanitise strings for console output
            static void sanitize(std::string &str) {
                // Symbols to replace ({fmt})
                static std::array<std::array<std::string, 2>, 2> symbols = {
                    std::array<std::string, 2>{"{", "{{"},
                    std::array<std::string, 2>{"}", "}}"}
                };

                // Replace all symbols
                for (const auto& pair : symbols) {
                    size_t pos = 0;
                    while ((pos = str.find(pair[0], pos)) != std::string::npos) {
                        str.replace(pos, 1, pair[1]); // Replace 1 char at pos with newString
                        pos += pair[1].length(); // Move past the replaced string
                    }
                }
            }

            // Keep track of print statistics
            static bool isFirstPrint = true;

            // Print report details
            void print() {
                // Track output data printing
                uint32_t color;
                int channel = 0; // [0 -> cout, 1 -> cerr]
                bool shouldColor = true;
                bool shouldPrompt = true;
                std::stringstream prompt;
                auto out = [&channel, &color, &shouldColor](std::string data) {
                    // {fmt}
                    sanitize(data);

                    // Print to the chosen output channel
                    if (shouldColor) {
                        INTERNAL_C_OUT(Console::Internal::color(data, color), channel);
                    } else {
                        INTERNAL_C_OUT(data, channel);
                    }
                };

                switch (IndividualReport::type) {
                case WARNING_REPORT:
                    color = Color::golden_rod;
                    channel = 1;
                    // Title
                    if (!IndividualReport::isContinuation) {
                        prompt << "[Warning]";
                    }
                    break;
                case CRITICAL_REPORT:
                    color = Color::crimson;
                    channel = 1;
                    // Title
                    if (!IndividualReport::isContinuation) {
                        prompt << "[Error]";
                    }
                    break;
                case FATAL_REPORT:
                    color = Color::crimson;
                    channel = 1;
                    // Title
                    if (!IndividualReport::isContinuation) {
                        prompt << "[Fatal Error]";
                    }
                    break;
                case ACTION_REPORT:
                    color = Color::sea_green;
                    channel = 1;
                    // Title
                    if (!IndividualReport::isContinuation) {
                        prompt << "[Action]";
                    }
                    break;
                case DEBUG_REPORT:
                    color = Color::blue_violet;
                    channel = 0;
                    // Title
                    if (!IndividualReport::isContinuation) {
                        prompt << "[Debug]";
                    }
                    break;
                default:
                    shouldColor = false;
                    channel = 0;
                    // No prompts
                    shouldPrompt = false;
                }

                // Print a new line for new reports
                if (!isFirstPrint) {
                    INTERNAL_C_OUT(std::endl, channel);
                }
                
                // Print report type info
                if (shouldPrompt) {
                    if (IndividualReport::stage.length() > 0) {
                        prompt << " (" << IndividualReport::stage << ") ";
                    } else {
                        prompt << " ";
                    }
                    out(prompt.str());
                }

                // TMP
                out(IndividualReport::messageStream.str());

                // Update print statistics
                if (isFirstPrint) {
                    isFirstPrint = false;
                }
            }
        }
    }
}
