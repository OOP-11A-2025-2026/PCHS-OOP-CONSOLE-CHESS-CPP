#include <iostream>
#include "cli/ChessCLI.h"

#ifdef _WIN32
#include <windows.h>
#endif

// Declaration of Darian's test function
int runDarianTests();

int main() {
    // Enable UTF-8 support on Windows
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    #endif

    // Run Darian's tests before starting the CLI
    // std::cout << "Running Darian's Tests..." << std::endl;
    // runDarianTests();
    // std::cout << "Tests finished. Starting CLI..." << std::endl << std::endl;

    try {
        // Create and start the chess CLI
        ChessCLI cli;
        cli.start();
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown fatal error occurred" << std::endl;
        return 1;
    }
}