/**
 * @brief
 * Manage os info
**/

#include "common/headers.hpp"

#if _WIN32
    #include <windows.h>
    #include <lm.h>
    #pragma comment(lib, "netapi32.lib")

    std::string getWindowsName() {
        LPWKSTA_INFO_102 wki;
        // Fetches the true major and minor OS version numbers
        if (NetWkstaGetInfo(NULL, 102, (LPBYTE*)&wki) == NERR_Success) {
            std::string tmp = (wki->wki102_platform_id == PLATFORM_ID_NT) ? "Windows NT" : "Windows";

            NetApiBufferFree(wki);

            return tmp;
        }

        return "?Windows?";
    }
    std::string getWindowsVersion() {
        LPWKSTA_INFO_102 wki;
        // Fetches the true major and minor OS version numbers
        if (NetWkstaGetInfo(NULL, 102, (LPBYTE*)&wki) == NERR_Success) {
            std::string tmp = std::to_string(wki->wki102_ver_major);
            tmp += ".";
            tmp += std::to_string(wki->wki102_ver_minor);

            NetApiBufferFree(wki);

            return tmp;
        }

        return "?v?";
    }
#elif __linux__
    #include <sys/utsname.h>
    std::string getLinuxOSName() {
        struct utsname buffer;
        if (uname(&buffer) == 0) {
            return (std::string) buffer.sysname;
        }

        return "?Linux?";
    }
    std::string getLinuxOSVersion() {
        struct utsname buffer;
        if (uname(&buffer) == 0) {
            return (std::string) buffer.release;
        }

        return "?v?";
    }
#elif __APPLE__
    #include <sys/sysctl.h>
    std::string getMacOSVersion() {
        char osversion[256];
        size_t size = sizeof(osversion);
        // Gets the Darwin kernel version
        if (sysctlbyname("kern.osrelease", osversion, &size, NULL, 0) == 0) {
            return (std::string) osversion;
        }

        return "?v?";
    }
#else
    // ???
#endif

std::string getOSName() {
    #if _WIN32
        return getWindowsName();
    #elif __linux__
        return getLinuxOSName();
    #elif __APPLE__
        return "macOS";
    #else
        return "Unknown OS";
    #endif
}

std::string getOSVersion() {
    #if _WIN32
        return getWindowsVersion();
    #elif __linux__
        return getLinuxOSVersion();
    #elif __APPLE__
        return getMacOSVersion();
    #else
        return "?v?";
    #endif
}
