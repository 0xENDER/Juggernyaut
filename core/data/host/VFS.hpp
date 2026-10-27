/**
 * @brief 
 * Virtual File System
**/

#pragma once

#include "common/headers.hpp"
#include "../dynamic.hpp" // JUG_DATA_API

// TO-DO: Implement an in-memory file cache

namespace Data {
    namespace Host {
        using FileId = uint32_t;

        class JUG_DATA_API VFS {
            public:
                VFS() {};
                virtual ~VFS() = default;

                // Basic functionality
                virtual bool readFile(const std::string& path, const std::string& output) = 0;
                virtual bool exists(const std::string& path) = 0;
                virtual bool isDirectory(const std::string& path) = 0;
                virtual const std::string canonical(const std::string& path) = 0; // Returns the canonical full path of a file
                virtual const std::string cwd() = 0;

                // Extended functionality
                const std::string getParentDirectory(const std::string& path) ; // e.g. c:\path\to\file.txt -> c:\path\to, c:\path\to -> c:\path,
                const std::string join(const std::string &path, const std::string &append) ;
                bool endsWith(const std::string &path, const std::string &end) ;
                const std::string filename(const std::string &path) ;
        };
    }
}
