function Component() {
    // Default constructor
}

Component.prototype.createOperations = function() {
    // Call default implementation first
    component.createOperations();

    if (systemInfo.productType === "windows") {
        // Create the Uninstall/Modify shortcut
        component.addOperation(
            "CreateShortcut",
            "@TargetDir@\\maintenancetool.exe",
            "@StartMenuDir@\\Uninstall Juggernyaut Toolchain.lnk",
            "description=Modify or Uninstall Juggernyaut"
        );
        // Add Start Menu Shortcut for Online Docs
        component.addOperation(
            "CreateShortcut",
            "explorer.exe",                                  // 1. Target executable
            "@StartMenuDir@\\Juggernyaut Documentation.lnk", // 2. Shortcut destination
            "arguments=https://ender.ing/docs/juggernyaut/", // 3. The URL to open
            "description=Open Online Documentation"          // 4. Description
            // "iconPath=@TargetDir@\\assets\\doc_icon.ico"        // 5. Icon (Optional)
        );
    }
}
