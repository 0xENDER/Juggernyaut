/**
 * @brief 
 * "Source code" store
**/

#include "SourceStore.hpp"

#include "../../common/utility.hpp"

// Basic C++ libraries
#include <algorithm>

namespace Data {
    namespace Store {
        // Entry
        void SourceStore::addEntry(SourceID entry) {
            std::vector<SourceID> &entries = this->entryPoints;
            if (std::ranges::find(entries, entry) == entries.end()) {
                entries.push_back(entry);
            }
        }
        void SourceStore::removeEntry(SourceID entry) {
            std::vector<SourceID> &entries = this->entryPoints;
            Common::Utility::fastVectorRemove(entries, entry);
        }
        void SourceStore::resetEntries() {
            std::vector<SourceID> &entries = this->entryPoints;
            entries.clear();
            entries.shrink_to_fit();
        }
        void SourceStore::visitEntries(const EntryCall entryCall) {
            std::vector<SourceID> &entries = this->entryPoints;
            for (auto entry : entries) {
                if (entryCall != nullptr) {
                    entryCall(entry);
                }
            }
        }

        // Sources
        Source& SourceStore::getSourceById(const SourceID &id) {

            std::unordered_map<SourceID, Source> &srcs = this->sources;

            return srcs.at(id);
        }

    }
}
