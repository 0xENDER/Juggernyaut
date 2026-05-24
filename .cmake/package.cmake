# Package Identity
set(CPACK_PACKAGE_NAME "Juggernyaut Toolchain")
set(CPACK_PACKAGE_VENDOR "Ender-ing")
set(CPACK_IFW_PRODUCT_URL "https://ender.ing/docs/juggernyaut/")
if (WIN32)
    set(CPACK_IFW_PACKAGE_ICON "${JUG_CMAKE_DIR}/installer/assets/jug_icon.ico")
elseif(APPLE)
    set(CPACK_IFW_PACKAGE_ICON "${JUG_CMAKE_DIR}/installer/assets/jug_icon.icns")
endif()

sanitize_version("${JUG_RELEASE_VERSION}" PROPER_RELEASE_VERSION)
set(CPACK_PACKAGE_VERSION "${PROPER_RELEASE_VERSION}")

set(CPACK_PACKAGE_INSTALL_DIRECTORY "Ender-ing/Juggernyaut")
set(CPACK_PACKAGE_FILE_NAME "juggernyaut-${JUG_RELEASE_VERSION}-${JUG_GENERATOR_PLATFORM}")

# Use IFW (QT Installer framework)
set(CPACK_GENERATOR "IFW")
set(CPACK_IFW_FRAMEWORK_VERSION 4.8.1)
# READ: https://cmake.org/cmake/help/latest/cpack_gen/ifw.html
# READ: https://doc.qt.io/qtinstallerframework/ifw-reference.html

# IFW Branding & Theming
set(CPACK_IFW_PACKAGE_TITLE "Juggernyaut Toolchain")
set(CPACK_IFW_PACKAGE_PUBLISHER "ender.ing")

set(CPACK_RESOURCE_FILE_LICENSE "${JUG_LICENSE_FILE}")

# Theming
set(CPACK_IFW_PACKAGE_WIZARD_STYLE "Modern")
# The left-side tall image (Classic setup wizard style)
set(CPACK_IFW_PACKAGE_WATERMARK "${JUG_CMAKE_DIR}/installer/assets/ifw_watermark.png")
# The top-right header logo
set(CPACK_IFW_PACKAGE_LOGO "${JUG_CMAKE_DIR}/installer/assets/ifw_backdrop_logo.png")
# The small window icon
set(CPACK_IFW_PACKAGE_WINDOW_ICON "${JUG_CMAKE_DIR}/installer/assets/jug_icon.png")
# Top bar banner
set(CPACK_IFW_PACKAGE_BANNER "${JUG_CMAKE_DIR}/installer/assets/ifw_banner.png")
# Colouring
set(CPACK_IFW_PACKAGE_TITLE_COLOR "#FFF3E3")
set(CPACK_IFW_PACKAGE_STYLE_SHEET "${JUG_CMAKE_DIR}/installer/assets/style.qss")

# Wizard styling
set(CPACK_IFW_PACKAGE_CONTROL_SCRIPT "${JUG_CMAKE_DIR}/installer/controller.qs")
set(CPACK_IFW_PACKAGE_WIZARD_DEFAULT_WIDTH 800)
set(CPACK_IFW_PACKAGE_WIZARD_DEFAULT_HEIGHT 600)
set(CPACK_IFW_PACKAGE_WIZARD_SHOW_PAGE_LIST ON)
# CPACK_IFW_PACKAGE_RESOURCES

# Behaviour
set(CPACK_IFW_PACKAGE_REMOVE_TARGET_DIR ON)
set(CPACK_IFW_PACKAGE_MAINTENANCE_TOOL_NAME "maintenancetool")
set(CPACK_IFW_PACKAGE_ALLOW_NON_ASCII_CHARACTERS OFF)
set(CPACK_IFW_PACKAGE_ALLOW_SPACE_IN_PATH ON)
set(CPACK_IFW_PACKAGE_DISABLE_COMMAND_LINE_INTERFACE OFF) # Allow CLI interactions
#set(CPACK_IFW_PACKAGE_RUN_PROGRAM "@TargetDir@/bin/jug-pck")
#set(CPACK_IFW_PACKAGE_RUN_PROGRAM_ARGUMENTS install std)
#set(CPACK_IFW_PACKAGE_RUN_PROGRAM_DESCRIPTION "Install the standard Juggernyaut standard library.")

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
if(WIN32)
    list(APPEND CPACK_COMPONENTS_ALL QSMenuShortcuts)
endif()

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
    EXPANDED
)
cpack_add_component(CmpJuggernyautCompiler
    DISPLAY_NAME "Juggernyaut Compiler"
    DESCRIPTION "Adds the Juggernyaut Compiler that allows you to build your final binaries."
    GROUP Toolchain
    REQUIRED TRUE
)
cpack_ifw_configure_component(CmpJuggernyautCompiler
    FORCED_INSTALLATION TRUE
    LICENSES 
        "Juggernyaut Toolchain License" "${JUG_LICENSE_FILE}"
)
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
    cpack_add_component(QSMenuShortcuts
        DISPLAY_NAME "Add Juggernyaut Toolchain Start Menu shortcuts"
        DESCRIPTION "Adds Start Menu shortcuts. (e.g. uninstaller shortcut)"
        GROUP SystemConfigs
    )
    cpack_ifw_configure_component(QSMenuShortcuts
        SCRIPT "${JUG_CMAKE_DIR}/installer/menu-shortcuts.qs"
    )
endif()

# Handle online fetching
# cpack_ifw_add_repository()
# cpack_ifw_update_repository()
# cpack_configure_downloads()
