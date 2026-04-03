/**
 * @brief
 * Manage file objects
**/

#include "FileStore.hpp"

#include "../console/console.hpp"

#include "files.hpp"

namespace Store {
    std::string FileStore::onFileRawRequest(const std::string &uri) {
        // Check if the file can be opened!
        if (!Files::isFileAccessible(uri)) {
            REPORT(Console::START_REPORT, Console::CRITICAL_REPORT, "Input file is non-existent or inaccessible!",
                Console::END_REPORT);

                return std::string("");
        }

        // Check if the file can be opened!
        if (!Files::isFileValid(uri)) {
            REPORT(Console::START_REPORT, Console::CRITICAL_REPORT,
            "Input file is corrupted or of an invalid type! (Must use a valid .jug file)",
            Console::END_REPORT);

            return std::string("");
        }

        // Get file content
        std::string content;
        if (Files::getFileContent(uri, content)) {
            REPORT(Console::START_REPORT, Console::CRITICAL_REPORT, "Error opening file: ", uri, Console::END_REPORT);
            return std::move(content);
        } else {
            return std::string("");
        }
    }
}
