# Package Identity
set(CPACK_PACKAGE_NAME "Juggernyaut Toolchain")
set(CPACK_PACKAGE_VENDOR "Ender-ing")

sanitize_version("${JUG_RELEASE_VERSION}" PROPER_RELEASE_VERSION)
set(CPACK_PACKAGE_VERSION "${PROPER_RELEASE_VERSION}")

set(CPACK_PACKAGE_INSTALL_DIRECTORY "Ender-ing/Juggernyaut")
set(CPACK_PACKAGE_FILE_NAME "juggernyaut-${JUG_RELEASE_VERSION}-${JUG_GENERATOR_PLATFORM}")

# Use IFW (QT Installer framework)
set(CPACK_GENERATOR "IFW")

# IFW Branding & Theming
set(CPACK_IFW_PACKAGE_TITLE "Juggernyaut Toolchain")
set(CPACK_IFW_PACKAGE_PUBLISHER "ender.ing")

set(CPACK_RESOURCE_FILE_LICENSE "${JUG_LICENSE_FILE}")
set(CPACK_IFW_MAINTENANCE_TOOL_NAME "maintenancetool")

# Theming
# The left-side tall image (Classic setup wizard style)
set(CPACK_IFW_PACKAGE_WATERMARK "${JUG_CMAKE_DIR}/installer/assets/ifw_watermark.png")
# The top-right header logo
set(CPACK_IFW_PACKAGE_LOGO "${JUG_CMAKE_DIR}/installer/assets/ifw_logo.png")
# The small window icon
set(CPACK_IFW_PACKAGE_WINDOW_ICON "${JUG_CMAKE_DIR}/installer/assets/jug_icon.png")

set(CPACK_COMPONENTS_ALL 
    # Project components
    CmpJuggernyautCompiler
    CmpJuggernyautServer
    CmpJuggernyautPackageManager
    CmpJuggernyautUnified

    # System components
    CmpSystemRuntimeLibs

    # QT script components
    QSPathSetup
)

# Separate components
set(CPACK_COMPONENTS_GROUPING IGNORE)

# Initialize CPack
include(CPack)
# Initialize CPackIFW (MUST come after include(CPack))
include(CPackIFW)

# Installation options

# Toolchain specifics
cpack_add_component_group(Toolchain
    DISPLAY_NAME "Toolchain Internals"
    DESCRIPTION "The core Juggernyaut executables and developer tools."
)
cpack_add_component(CmpJuggernyautCompiler
    DISPLAY_NAME "Juggernyaut Compiler"
    DESCRIPTION "Adds the Juggernyaut Compiler that allows you to build your final binaries."
    GROUP Toolchain
    REQUIRED TRUE
)
#cpack_ifw_configure_component(CmpJuggernyautCompiler
#    FORCED_INSTALLATION TRUE
#)
cpack_add_component(CmpJuggernyautServer
    DISPLAY_NAME "Juggernyaut Language Server"
    DESCRIPTION "Adds the Juggernyaut Language Server, which is necessary for IDE integration."
    GROUP Toolchain
)
cpack_add_component(CmpJuggernyautPackageManager
    DISPLAY_NAME "Juggernyaut Package Manager"
    DESCRIPTION "Adds the Juggernyaut Package Manager, which allows for easy management of global Juggernyaut librarie."
    GROUP Toolchain
    DISABLED TRUE
)
cpack_add_component(CmpJuggernyautUnified
    DISPLAY_NAME "Juggernyaut unified command"
    DESCRIPTION "Adds a unified 'jug' command that can call all other toolchain components."
    GROUP Toolchain
    DISABLED TRUE
)
cpack_ifw_configure_component(CmpJuggernyautUnified
    DEPENDS Toolchain.CmpJuggernyautCompiler Toolchain.CmpJuggernyautServer Toolchain.CmpJuggernyautPackageManager
)

# System
cpack_add_component_group(SystemConfigs
    DISPLAY_NAME "System Configurations"
    DESCRIPTION "Local system configurations that can help make your experience better."
)
cpack_add_component(QSPathSetup
    DISPLAY_NAME "Add the Juggernyaut Toolchain bin to System PATH"
    DESCRIPTION "Allows you to run toolchain commands (e.g. 'jug') from any terminal."
    GROUP SystemConfigs
)
cpack_ifw_configure_component(QSPathSetup
    SCRIPT "${JUG_CMAKE_DIR}/installer/path-setup.qs"
)
cpack_add_component(CmpSystemRuntimeLibs
    DISPLAY_NAME "Required system runtime components"
    DESCRIPTION "Adds system runtime components that are required by some toolchain binaries. (Unselect only if you know what you're doing!)"
    GROUP SystemConfigs
)
if(WIN32)
    list(APPEND CPACK_COMPONENTS_ALL QSMenuShortcuts)
    cpack_add_component(QSMenuShortcuts
        DISPLAY_NAME "Add Juggernyaut Toolchain Start Menu shortcuts"
        DESCRIPTION "Adds Start Menu shortcuts. (e.g. uninstaller shortcut)"
        GROUP SystemConfigs
    )
    cpack_ifw_configure_component(QSMenuShortcuts
        SCRIPT "${JUG_CMAKE_DIR}/installer/menu-shortcuts.qs"
    )
endif()
