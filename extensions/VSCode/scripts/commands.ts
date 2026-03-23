/**
 * 
 * Take care of Omniarium extension commands!
 * 
**/

// Imports
import { window, commands, env, Uri, ExtensionContext } from "vscode";
import { deactivate, outputChannel } from "./server";

// Constants
const DOCS_URL = 'https://ender.ing/docs/juggernyaut/';

// Message test
const docs = commands.registerCommand('juggernyaut.docs', () => {
    const url = Uri.parse(DOCS_URL);
    env.openExternal(url);
});
const server = {
    stop: commands.registerCommand('juggernyaut.server.stop', async () => {
        window.showInformationMessage("Deactivating the Juggernyaut language server...")
        await deactivate();
        window.showInformationMessage("The Juggernyaut language server has been deactivated.")
    }),
    output: commands.registerCommand('juggernyaut.server.debug', async () => {
        outputChannel.show(true);
    })
};

export function register(context: ExtensionContext) {
    context.subscriptions.push(docs, server.stop);
}