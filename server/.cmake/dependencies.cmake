message(STATUS "[DEPENDENCIES] Checking dependencies...")

# lsp-framework
set(LSP_FRAMEWORK_LIB_VERSION 1.3.1)
option(USE_INSTALLED_LSP_FRAMEWORK "Ignore or use installed lsp-framework" OFF) # Default OFF
if(USE_INSTALLED_LSP_FRAMEWORK)
    find_package(lsp ${LSP_FRAMEWORK_LIB_VERSION} EXACT REQUIRED)
endif()
if(lsp)
    message(STATUS "[DEPENDENCIES] lsp-framework library is present!")
else()
    # Download lsp-framework
    message(STATUS "[DEPENDENCIES] Fetching lsp-framework...")
    set(JUG_DEP_LSP_FRAMEWORK_LIB_PATH ${JUG_DEPENDENCIES_DIR}/lsp-framework)
    if(EXISTS ${JUG_DEP_LSP_FRAMEWORK_LIB_PATH}/CMakeLists.txt)
        FetchContent_Declare(lsp
            GIT_TAG ${LSP_FRAMEWORK_LIB_VERSION}
            SOURCE_DIR ${JUG_DEP_LSP_FRAMEWORK_LIB_PATH})
    else()
        FetchContent_Declare(lsp
            GIT_REPOSITORY https://github.com/leon-bckl/lsp-framework.git
            GIT_TAG ${LSP_FRAMEWORK_LIB_VERSION}
            SOURCE_DIR ${JUG_DEP_LSP_FRAMEWORK_LIB_PATH})
    endif()
    FetchContent_MakeAvailable(lsp)
endif()
# Turn off the "unused-result" warning for the lsp target specifically
if(TARGET lsp)
    target_compile_options(lsp PRIVATE -Wno-unused-result -Wno-shadow)
endif()
# Post-build cleanup
add_custom_target(LSPCleanup ALL
                    COMMAND ${CMAKE_COMMAND}
                           -E rm -f ./lspgen*
                    WORKING_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
add_dependencies(LSPCleanup lsp)
