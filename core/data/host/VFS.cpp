/**
 * @brief 
 * Virtual File System
**/

#include "VFS.hpp"

#include <filesystem>
// Make sure to only use <filesystem> functions that do not interact with the system/host!

namespace Data {
    namespace Host {
        const std::string VFS::getParentDirectory(const std::string& path) {
            return std::filesystem::path(path).parent_path().string();
        }
        const std::string VFS::join(const std::string &path, const std::string &append) {
            return (std::filesystem::path(path) / std::filesystem::path(append)).string();
        }
        bool VFS::endsWith(const std::string &path, const std::string &end) {
            if (path.length() >= end.length()) {
                return path.compare(path.length() - end.length(), end.length(), end) == 0;
            }
            return false;
        }
        const std::string VFS::filename(const std::string& path) {
            return std::filesystem::path(path).filename().string();
        }
    }
}
