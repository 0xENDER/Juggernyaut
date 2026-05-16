# Manage C++ compiler settings
include(${JUG_CMAKE_DIR}/compiler/cpp.cmake)

# CMake functions & macros
if (NOT DEFINED target_sources_search)
    include(${JUG_CMAKE_DIR}/utility.cmake)
endif()

# Manage binary output
if (NOT DEFINED JUG_BINARY_MODE)
    include(${JUG_CMAKE_DIR}/build-output.cmake)
endif()

# Manage shared dependencies
include(${JUG_CMAKE_DIR}/dependencies.cmake)

# Diagnostic codes
include(${JUG_CMAKE_DIR}/common.cmake)

# Diagnostic codes
include(${JUG_CMAKE_DIR}/diagnostics.cmake)
