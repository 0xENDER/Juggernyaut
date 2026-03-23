import { window, workspace, Uri, ExtensionContext, OutputChannel  } from 'vscode';
import {
    LanguageClient,
    LanguageClientOptions,
    ServerOptions,
    TransportKind,
    Executable} from 'vscode-languageclient/node';
import * as sdk from "./sdk"
import { getCommand } from './platform';

export let client: LanguageClient | undefined = undefined;

async function start() {
    // Define the path to the executable!
    const sdkPath: string = await sdk.getPath();
    if(sdkPath == "") {
        return;
    }
    const binPath: string = Uri.joinPath(Uri.file(sdkPath), "bin").fsPath;
    const command = await getCommand(binPath, "jug-lsp");

    // 2. Define how the server is launched
    const executable: Executable = {
        command: command,
        transport: TransportKind.stdio,
        args: [] // Add any command-line arguments your C++ server expects here
    };

    const serverOptions: ServerOptions = {
        run: executable,
        debug: executable // You can pass different args here if you have a debug mode
    };

    // 3. Define the client options
    const clientOptions: LanguageClientOptions = {
        // Register the server for your custom language. 
        // 'yourLanguageId' must match the language id defined in your package.json
        documentSelector: [{ scheme: 'file', language: 'juggernyaut' }]
    };

    // Create the language client and start the client.
    client = new LanguageClient(
        'juggernyaut',
        'Juggernyaut Language Server',
        serverOptions,
        clientOptions
    );
    client.start();
}

// Restrict language server runtime
let isChangingServerState = false;
async function manageServerLifecycle() {
    if (isChangingServerState) {
        return;
    }
    const hasActiveFiles = workspace.textDocuments.some(
        (doc) => doc.languageId === 'juggernyaut' 
    );

    // Start/stop the server
    if (hasActiveFiles && client == undefined) { // Start
        isChangingServerState = true;
        await start();
        isChangingServerState = false;
        logInfo('Starting the Juggernyaut language server...');
    } else if (!hasActiveFiles && client != undefined) { // Stop
        isChangingServerState = true;
        await client.stop();
        client = undefined; // Clear the reference so it can be restarted
        isChangingServerState = false;
        logInfo('Terminating the Juggernyaut language server...');
    }
}

// Prevent excessive updates
let debounceTimeout: NodeJS.Timeout | undefined;
function debouncedLifecycleCheck() {
    if (debounceTimeout) {
        clearTimeout(debounceTimeout);
    }
    
    // Start a new timer
    debounceTimeout = setTimeout(() => {
        manageServerLifecycle();
    }, 150);
}

export function activate(context: ExtensionContext) {
    manageServerLifecycle();
    context.subscriptions.push(
        workspace.onDidChangeConfiguration(debouncedLifecycleCheck),
        workspace.onDidOpenTextDocument(debouncedLifecycleCheck),
        workspace.onDidCloseTextDocument(debouncedLifecycleCheck),
        workspace.onDidRenameFiles(debouncedLifecycleCheck),
        workspace.onDidCreateFiles(debouncedLifecycleCheck),
        workspace.onDidDeleteFiles(debouncedLifecycleCheck),
        workspace.onDidChangeWorkspaceFolders(debouncedLifecycleCheck)
    );
}

export async function deactivate() {
    logInfo("Deactivation Request received!")
    if (client == undefined) {
        logInfo("Server is already inactive!")
        return undefined;
    }
    if (isChangingServerState == true) {
        await new Promise(resolve => setTimeout(resolve, 150));
        return await deactivate();
    }
    isChangingServerState = true;
    await client.stop();
    client = undefined; // Clear the reference so it can be restarted
    isChangingServerState = false;
    logInfo("Server has been successfully deactivated!")
}

// Debugging!

export const outputChannel: OutputChannel  = window.createOutputChannel('Juggernyaut Language Server (Debug)');

function logInfo(message: string) {
    // 2. Generate the timestamp to match your screenshot exactly.
    // toLocaleString() natively produces the "M/DD/YYYY, H:MM:SS AM/PM" format.
    const timestamp = new Date().toLocaleString('en-US');
    
    // 3. Write the formatted string to the output panel
    outputChannel.appendLine(`[${timestamp}] ${message}`);
}

function logError(message: string, error?: any) {
    const timestamp = new Date().toLocaleString('en-US');
    outputChannel.appendLine(`[${timestamp}] ERROR: ${message}`);
    if (error) {
        outputChannel.appendLine(String(error));
    }
}
