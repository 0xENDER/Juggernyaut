/**
 * @brief
 * Manage the compiler's basic info
**/

#include "info.hpp"

#include "os.hpp"

namespace Base {
    namespace Info {
        // Version info
        const std::string version = MAIN_TARGET_BINARY_VERSION;

        // Installation info
        const std::string arch =
        #if defined(__x86_64__) || defined(_M_X64)
            "x86_64 (64-bit Intel/AMD)";
        #elif defined(__i386__) || defined(_M_IX86)
            "x86 (32-bit Intel/AMD)";
        #elif defined(__aarch64__) || defined(_M_ARM64)
            "ARM64 (64-bit ARM)";
        #elif defined(__arm__) || defined(_M_ARM)
            "ARM (32-bit ARM)";
        #else
            "Unknown Architecture";
        #endif
        const std::string build = MAIN_BINARY_TYPE;

        // OS info
        const std::string osName = getOSName();
        const std::string osVersion = getOSVersion();
    }
}
