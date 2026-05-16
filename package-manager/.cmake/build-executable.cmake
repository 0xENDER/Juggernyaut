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
    )
# Regular expressions lib
if(TARGET pcre)
    target_link_libraries(JuggernyautPackageManager PRIVATE pcre)
endif()
# Compression
if(TARGET zlib)
    target_link_libraries(JuggernyautPackageManager PRIVATE zlib)
endif()
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
elseif(APPLE)
    # macOS
    # Find required Apple Frameworks and system libraries
    find_library(CORE_FOUNDATION CoreFoundation)
    find_library(SECURITY Security)
    find_library(ICONV iconv)
    
    target_link_libraries(JuggernyautPackageManager PRIVATE
        ${CORE_FOUNDATION} # Apple base APIs
        ${SECURITY}        # Keychain and Cryptography
        ${ICONV}           # Character encoding conversion
        resolv             # DNS resolution
    )
elseif(UNIX AND NOT APPLE)
    # Linux
    # Find required POSIX packages
    find_package(Threads REQUIRED)

    set(OPENSSL_USE_STATIC_LIBS TRUE)
    find_package(OpenSSL REQUIRED)

    target_link_libraries(JuggernyautPackageManager PRIVATE
        Threads::Threads   # POSIX Threads (pthread)
        OpenSSL::SSL       # Secure Sockets
        OpenSSL::Crypto    # Cryptography
        dl                 # Dynamic Linker (for loading shared objects)
        rt                 # Real-time extensions (sometimes required by internal timers)
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
