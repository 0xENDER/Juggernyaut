/**
 * @brief
 * {fmt} wrapper
**/

#pragma once

// --- Turn off warnings for {fmt} headers ---
#ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable : 4834 6240 6326 6294)
#elif defined(__GNUC__)
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-result -Wlogical-op -Wunreachable-code"
#elif defined(__clang__)
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wunused-result -Wconstant-logical-operand -Wtautological-compare -Wunreachable-code"
#endif

#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/std.h>

// --- Restore warnings back to normal for your code ---
#ifdef _MSC_VER
    #pragma warning(pop)
#elif defined(__GNUC__)
    #pragma GCC diagnostic pop
#elif defined(__clang__)
    #pragma clang diagnostic pop
#endif
