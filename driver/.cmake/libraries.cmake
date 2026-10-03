# PIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(BUILD_SHARED_LIBS ON)

# Create a library
add_library(JuggernyautDriverLibrary SHARED)
target_sources_search(JuggernyautDriverLibrary ${JUG_DRIVER_SOURCE_DIR}/*.cpp FALSE)
# Expose library exports
target_compile_definitions(JuggernyautDriverLibrary PRIVATE JUG_DRIVER_LIBRARY_EXPORTS)
# Attach manifest data
attach_manifest_data(JuggernyautDriverLibrary ${JUG_DRIVER_MANIFEST_FILE} TRUE)
# Add compiler flags
add_internal_target_cxx_flags(JuggernyautDriverLibrary FALSE)
# Dependencies
jug_common(JuggernyautDriverLibrary)
add_dependencies(JuggernyautDriverLibrary JuggernyautSessionLibrary JuggernyautDataLibrary)
target_link_libraries(JuggernyautDriverLibrary PUBLIC JuggernyautSessionLibrary JuggernyautDataLibrary)
target_include_directories(JuggernyautDriverLibrary SYSTEM PRIVATE ${TOMLPP_INCLUDE_DIR})
