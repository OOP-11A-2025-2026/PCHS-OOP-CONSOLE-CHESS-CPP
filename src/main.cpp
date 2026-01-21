#include <iostream>
#include "cli/ChessCLI.h"

int main() {
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