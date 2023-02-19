#include <iostream>
#include <getopt.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <wasmedge/wasmedge.h>
#include "./wasmedge_function.cpp"

using namespace std;

void printUsage() {
    cerr << "Error: `wasm path` is required;" << endl;
    cout << "Usage: tool [version] [run] [wasm path] [arguments]" << endl;
}

// Parse arguments
int parseArgs( int argc, char *argv[], string &wasmPath, bool &versionFlag, vector<string> &args ){
    // tool [version] [run] [wasm path] [arguments]

    if (argc < 2) {
        printUsage();
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "version") {
            versionFlag = true;
        } else if (arg == "run") {
            continue;
        } else if (arg.find(".wasm") != string::npos) {
            wasmPath = arg;
        } else {
            args.push_back(arg);
        }
    }

    // If version flag is set, ignore other flags
    // If version flag is not set, check if wasm path is given
    if (!versionFlag && wasmPath.empty()) {
        printUsage();
        return 1;
    }
    else {
        return 0;
    }
}


int main(int argc, char *argv[]) {
    string wasmPath;
    bool versionFlag = false;
    vector<string> args;


    // Parse arguments
    cout << "Parsing arguments" << endl;
    cout << "Argument number: " << argc << endl << endl;
    if (parseArgs(argc, argv, wasmPath, versionFlag, args) != 0) {
        return 1;
    }

    // If version flag is set, ignore other flags
    if (versionFlag) {
        printVersion();
        return 0;
    }

    // Run VM
    if (vmMain(wasmPath, args) != 0) {
        return 1;
    }

    
    return 0;

}