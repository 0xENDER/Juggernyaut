message(STATUS "[INSTALL] Setting up target installs...")

# General
install(TARGETS JuggernyautCommonLibrary JuggernyautCatConsoleLibrary
    EXPORT JuggernyautToolchain
    RUNTIME DESTINATION bin    # Executables (.exe) AND Windows DLLs (.dll) go here
    LIBRARY DESTINATION lib    # Linux (.so) and macOS (.dylib) shared libs go here
)

# Core
install(TARGETS JuggernyautDiagnosticsLibrary JuggernyautDataLibrary
        JuggernyautParserLibrary JuggernyautSessionLibrary JuggernyautManagerLibrary
    EXPORT JuggernyautToolchain
    RUNTIME DESTINATION bin    # Executables (.exe) AND Windows DLLs (.dll) go here
    LIBRARY DESTINATION lib    # Linux (.so) and macOS (.dylib) shared libs go here
)

# Configs
install(TARGETS JuggernyautConfigsLibrary
    EXPORT JuggernyautToolchain
    RUNTIME DESTINATION bin    # Executables (.exe) AND Windows DLLs (.dll) go here
    LIBRARY DESTINATION lib    # Linux (.so) and macOS (.dylib) shared libs go here
)

# External deps (without installs/excluded)
if(TARGET mimalloc)
    install(TARGETS mimalloc
        RUNTIME DESTINATION bin    # Executables (.exe) AND Windows DLLs (.dll) go here
        LIBRARY DESTINATION lib    # Linux (.so) and macOS (.dylib) shared libs go here
    )
    if(WIN32)
        install(CODE "
            file(GLOB MIMALLOC_REDIRECT_FILES \"${JUG_OUT_FINAL_DIR}/bin/mimalloc-redirect*.dll\")
            
            if(MIMALLOC_REDIRECT_FILES)
                message(STATUS \"[INSTALL] Found runtime redirectors, copying...\")
                file(INSTALL TYPE PROGRAM 
                     FILES \${MIMALLOC_REDIRECT_FILES} 
                     DESTINATION \"\${CMAKE_INSTALL_PREFIX}/bin\"
                )
            else()
                message(FATAL_ERROR \"[INSTALL] No mimalloc-redirect DLLs found in ${JUG_OUT_FINAL_DIR}/bin at install time!\")
            endif()
        ")
    endif()
endif()
install(TARGETS antlr4_shared
    RUNTIME DESTINATION bin    # Executables (.exe) AND Windows DLLs (.dll) go here
    LIBRARY DESTINATION lib    # Linux (.so) and macOS (.dylib) shared libs go here
)
# Copy ANTLR4 binaries
#add_custom_command(
#    TARGET JuggernyautToolchain
#    POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}"
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:antlr4_shared> "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}"
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_LINKER_FILE:antlr4_shared> "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")

# Executables
install(TARGETS JuggernyautCompiler JuggernyautServer JuggernyautPackageManager
    EXPORT JuggernyautToolchain
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)

# Commands
install_command_symlink(JuggernyautCompiler jug-cmp)
install_command_symlink(JuggernyautServer jug-lsp)
install_command_symlink(JuggernyautPackageManager jug-pck)

# License
install(FILES ${JUG_LICENSE_FILE}
    DESTINATION .
)

# Cleanup
if(WIN32)
    set(INSTALL_STATIC_LIB_EXT lib)
else()
    set(INSTALL_STATIC_LIB_EXT a)
endif()
install(CODE "
    message(STATUS \"[POST-INSTALL] Cleaning up extra components...\")
    
    # Dirs
    file(REMOVE_RECURSE \"${JUG_DIST_FINAL_DIR}/include\")
    file(REMOVE_RECURSE \"${JUG_DIST_FINAL_DIR}/share\")
    file(REMOVE_RECURSE \"${JUG_DIST_FINAL_DIR}/bin/cmake\")
    file(REMOVE_RECURSE \"${JUG_DIST_FINAL_DIR}/bin/pkgconfig\")
    
    # Static libs
    file(GLOB_RECURSE static_libs \"${JUG_DIST_FINAL_DIR}/*.${INSTALL_STATIC_LIB_EXT}\")
    if(static_libs)
        file(REMOVE \${static_libs})
    endif()
    
    message(STATUS \"[POST-INSTALL] Cleanup complete!\")
")

# Wrapper
add_executable(jug ${JUG_MAIN_DIR}/wrapper.cpp)

install(TARGETS jug
    EXPORT JuggernyautToolchain
    RUNTIME DESTINATION bin
)