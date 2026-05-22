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
    }
}
