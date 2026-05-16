message(STATUS "[INSTALL] Setting up target installs...")

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
install(TARGETS mimalloc antlr4_shared
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

# dev commands
generate_command(cleanup)

if(WIN32)
    # Windows: Execute the .bat file
    install(CODE "
        message(STATUS \"[POST-INSTALL] Running Windows batch script...\")
        execute_process(
            COMMAND cmd.exe /c \"${JUG_DIST_FINAL_DIR}.cleanup.bat\"
            RESULT_VARIABLE script_res
        )
        if(NOT script_res EQUAL 0)
            message(WARNING \"Post-install script failed with code: \${script_res}\")
        endif()
    ")
else()
    # Unix/macOS: Execute the .sh file
    install(CODE "
        message(STATUS \"[POST-INSTALL] Running Unix shell script...\")
        execute_process(
            COMMAND bash \"${JUG_DIST_FINAL_DIR}.cleanup.sh\"
            RESULT_VARIABLE script_res
        )
        if(NOT script_res EQUAL 0)
            message(WARNING \"Post-install script failed with code: \${script_res}\")
        endif()
    ")
endif()
