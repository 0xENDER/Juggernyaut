/**
 * @brief 
 * Front-End Session
**/

#include "Session.hpp"

namespace Session {
    void Session::addEntryPoint(const File* file) {
        CompilationUnitState unit = {file = file};

        // Update entry status
        this->entryPoints.push_back(file->id);
        unit.isEntryPoint = true;

        // Move unit
        this->units[file->id] = std::move(unit);
    }
}
