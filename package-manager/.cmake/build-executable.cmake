# Add executable and include relevant files
message(STATUS "[BUILD] Adding executable target 'JuggernyautPackageManager'...")
add_executable(
    JuggernyautPackageManager ${JUG_PACKAGE_MANAGER_MAIN_CPP_PATH}
)

# Add internal files
#target_sources_search(JuggernyautPackageManager ${JUG_PACKAGE_MANAGER_SOURCE_DIR}/???/*.cpp FALSE)

# Link libraries
jug_common(JuggernyautPackageManager)
# LINK THE TOML READER!

# Link external libraries
# libgit2
add_dependencies(JuggernyautPackageManager libgit2)
target_link_libraries(JuggernyautPackageManager PRIVATE libgit2)
# Link internal libgit2 libraries
target_link_libraries(JuggernyautPackageManager 
    PRIVATE 
        util            # libgit2's internal fs/utils (often required in v1.8+)
        llhttp          # The bundled HTTP parser
        xdiff           # The bundled diff engine
        zlib            # Compression (if bundled/used)
        pcre            # Regular expressions lib
    )
# Link system libraries on Windows
if(WIN32)
    target_link_libraries(JuggernyautPackageManager PRIVATE
        ws2_32           # Winsock 2 (networking)
        secur32          # Security functions (SSPI)
        rpcrt4           # RPC runtime (UUID generation)
        advapi32         # Windows API (registry, crypto)
        winhttp          # WinHTTP
        kernel32         # Kernel functions
        crypt32          # Cryptography
    )
endif()

# Handle dynamic libraries
target_link_directories(JuggernyautPackageManager PRIVATE "$<TARGET_FILE_DIR:JuggernyautPackageManager>")

# Attach manifest data
# The first use of the "attach_manifest_data" function must be for the main executable!
attach_manifest_data(JuggernyautPackageManager ${JUG_PACKAGE_MANAGER_MANIFEST_FILE} TRUE)

# Re-do symbolic linking (POST BUILD)
manage_symbolic_links(JuggernyautPackageManager "jug-pck")

# Add compiler flags
add_internal_target_cxx_flags(JuggernyautPackageManager FALSE)

# Add debug flags
include(${JUG_CMAKE_DIR}/debug-mode.cmake)
