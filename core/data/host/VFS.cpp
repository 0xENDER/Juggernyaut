/**
 * @brief 
 * Virtual File System
**/

#include "VFS.hpp"

#include <filesystem>
// Make sure to only use <filesystem> functions that do not interact with the system/host!

namespace Data {
    namespace Host {
        // Technical
        File* VFS::addEmptyFile(const std::string& canonical) {
            // Generate a file object
            FilePtr file = std::make_unique<File>();
            file->canonical = canonical;

            // Update modification time
            if (this->trackModificationTime) {
                file->lastModified = this->_lastModified(canonical);
            }

            // Save the file
            FileId fileId = this->cache.size() + this->FILE_ID_OFFSET;
            file->id = fileId;
            this->cache.push_back(std::move(file));

            // Update URI index
            this->index[canonical] = fileId;

            return this->cache[fileId - this->FILE_ID_OFFSET].get();
        }

        // External Management functions (Should not be called within the front-end!)
        // Note: newContent will be reset!
        void VFS::updateFile(const std::string& path, std::string &newContent) {
            const std::string canonicalUri = this->_canonical(path);

            File* file;
            // Check if the file already exists
            auto it = this->index.find(canonicalUri);
            if (it != this->index.end()) {
                auto fileId = it->second;

                file = this->cache[fileId - this->FILE_ID_OFFSET].get();

                // Update modification time
                if (this->trackModificationTime) {
                    file->lastModified = this->_lastModified(canonicalUri);
                }
            } else {
                file  = this->addEmptyFile(canonicalUri);
            }

            // Update content
            file->content = std::move(newContent);
            newContent = ""; // For safety!
            file->isValid = true;

            // Bump up the version
            file->version++;
        }
        void VFS::reloadFile(const std::string& path) {
            const std::string canonicalUri = this->_canonical(path);

            File* file;
            // Check if the file already exists
            auto it = this->index.find(canonicalUri);
            if (it != this->index.end()) {
                auto fileId = it->second;

                file = this->cache[fileId - this->FILE_ID_OFFSET].get();

                // Update modification time
                if (this->trackModificationTime) {
                    file->lastModified = this->_lastModified(canonicalUri);
                }
            } else {
                file = this->addEmptyFile(canonicalUri);
            }

            // Update content
            file->isValid = this->_readFile(canonicalUri, file->content);

            // Bump up the version
            file->version++;
        }

        // Basic functionality
        const File* VFS::readFile(const std::string& path) {
            const std::string canonicalUri = this->_canonical(path);

            // Check if the file already exists
            auto it = this->index.find(canonicalUri);
            if (it != this->index.end()) {
                auto fileId = it->second;

                return this->cache[fileId - this->FILE_ID_OFFSET].get();
            } else {
                // Generate a file object
                File* file = this->addEmptyFile(canonicalUri);

                // Request and load the file!
                // Diagnostic generation is handled by third parties!
                file->isValid = this->_readFile(canonicalUri, file->content);

                // Bump up the version
                file->version++;

                return file;
            }
        }
        const File* VFS::readFile(const FileId& id) {
            FileId indx = id - this->FILE_ID_OFFSET;
            if (id >= this->FILE_ID_OFFSET &&  indx < this->cache.size()) {
                return this->cache[indx].get();
            } else {
                return nullptr;
            }
        }
        bool VFS::exists(const std::string& path) {
            return this->_exists(path);
        }
        bool VFS::isDirectory(const std::string& path) {
            return this->_isDirectory(path);
        }

        // Extended functionality
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
