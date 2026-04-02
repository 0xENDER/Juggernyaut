/**
 * @brief 
 * "Source code" instance
**/

#include "Source.hpp"

#include "../../common/utility.hpp"

// Basic C++ libraries
#include <algorithm>

#include "SourceStore.hpp"

namespace Data {
    namespace Store {
        // Init
        Source::Source(SourceStore *srcStore, std::string srcUri)
            : store(srcStore), uri(srcUri) {
            this->id = ++(this->store->lastID);
        }
        SourceID Source::getID() {
            return this->id;
        }


        // Tracking
        void Source::requestRawUpdate() {
            this->shouldUpdateRawContent = true;
        }

        // Dependency tracking
        void Source::addSourceDependency(SourceID dep) {
            std::vector<SourceID> &srcs = this->neededSources;
            if (std::ranges::find(srcs, dep) == srcs.end()) {
                srcs.push_back(dep);
            }
        }
        void Source::removeSourceDependency(SourceID dep) {
            std::vector<SourceID> &srcs = this->neededSources;
            Common::Utility::fastVectorRemove(srcs, dep);
        }
        void Source::resetSourceDependencies() {
            std::vector<SourceID> &srcs = this->neededSources;
            srcs.clear();
            srcs.shrink_to_fit();
        }

        // Raw content
        void Source::fetchRawContent() {
            SourceStore *srcStore = this->store;
            this->rawContent = std::move(srcStore->onFileRawRequest(this->uri));
        }
        const std::string& Source::getRawContent() {
            if (this->shouldUpdateRawContent) {
                this->fetchRawContent();
            }
            return this->rawContent;
        }

        void Source::setIsEntryPoint(const bool state) {
            this->isEntryPoint = state;
        }
    }
}
