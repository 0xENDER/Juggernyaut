/**
 * @brief 
 * String-related functions
**/

#pragma once

template <typename Target, typename Source>
inline bool is_type(Source* obj) {
    // We add const to the target so it works with const Source pointers
    return dynamic_cast<const Target*>(obj) != nullptr;
}
