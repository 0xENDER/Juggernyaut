/**
 * @brief
 * Fix ANTLR4 inclusion
**/

#pragma once

// 1. Prevent Windows from injecting max/min macros
#ifndef NOMINMAX
#define NOMINMAX
#endif

// 2. Include the actual ANTLR runtime
#ifndef JUG_ANTLR4FIX
#include "antlr4-runtime.h"
#endif

// 3. Clean up standard library macro poisoning if it happened
#ifdef EOF
#undef EOF
#endif

#ifndef JUG_ANTLR4FIX
#define JUG_ANTLR4FIX
#endif
