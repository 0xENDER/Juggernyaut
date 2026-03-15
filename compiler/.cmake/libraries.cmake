# PIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(BUILD_SHARED_LIBS ON)

# Create a library from /base
add_library(JugCompilerBaseLibrary STATIC)
target_sources_search(JugCompilerBaseLibrary ${JUG_COMPILER_SOURCE_DIR}/base/*.cpp FALSE)
# Attach manifest data
attach_manifest_data(JugCompilerBaseLibrary ${JUG_COMPILER_MANIFEST_FILE})
# Add compiler flags
add_internal_target_cxx_flags(JugCompilerBaseLibrary FALSE)

# Create a library from /console
add_library(JugCompilerConsoleLibrary STATIC)
target_sources_search(JugCompilerConsoleLibrary ${JUG_COMPILER_SOURCE_DIR}/console/*.cpp FALSE)
# Attach manifest data
attach_manifest_data(JugCompilerConsoleLibrary ${JUG_COMPILER_MANIFEST_FILE})
# Add compiler flags
add_internal_target_cxx_flags(JugCompilerConsoleLibrary FALSE)
# Link other libraries to the library
add_dependencies(JugCompilerConsoleLibrary fmt::fmt)
target_link_libraries(JugCompilerConsoleLibrary PUBLIC fmt::fmt)

# Compiler libraries
set(COMPILER_LIBRARIES
    JugCompilerBaseLibrary
    JugCompilerConsoleLibrary
)