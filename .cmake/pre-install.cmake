message(STATUS "[INSTALL] Setting up install configs...")

include(GNUInstallDirs)

file(TO_CMAKE_PATH "${JUG_DIST_DIR}/${JUG_BINARY_MODE}/${JUG_BINARY_PLATFORM}" JUG_DIST_FINAL_DIR)
set(CMAKE_INSTALL_PREFIX "${JUG_DIST_FINAL_DIR}" CACHE PATH "Installation Prefix" FORCE)

set(CMAKE_INSTALL_BINDIR "bin" CACHE PATH "" FORCE)
if (WIN32)
    set(CMAKE_INSTALL_LIBDIR "bin" CACHE PATH "" FORCE)
else()
    set(CMAKE_INSTALL_LIBDIR "lib" CACHE PATH "" FORCE)
endif()

# Fix RPATH issues
if(UNIX AND NOT APPLE)
    # Define the relative path from the executable (bin/) to the libraries (lib/)
    set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")

    # Ensure that dependency libraries also use RPATH to find their own dependencies
    # if they sit in the same folder.
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()

# External deps
set(ANTLR_BUILD_SHARED ON CACHE BOOL "" FORCE)
set(LSP_INSTALL OFF CACHE BOOL "" FORCE)
