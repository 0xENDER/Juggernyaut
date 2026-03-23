/**
 *
 * Start the extension!
 *
**/

// Imports
import * as vscode from "vscode";
import * as commands from "./commands";
import * as server from "./server"

// Lightweight extension activation! (for web browsers!)
export function lightweightActivation (context: vscode.ExtensionContext){
    // Register light commands!
    commands.register(context)
}

// Activate the extension!
export async function activate(context: vscode.ExtensionContext) {
    // Do lightweight activation first
    lightweightActivation(context);
    // Start the language server
    server.activate(context);
}

export async function deactivate(): Promise<Thenable<void>> {
    if (server.client == undefined) {
        return undefined;
    }
    return await server.deactivate();
}
