#include "input/PGNHandler.h"
#include "input/MoveParser.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <cctype>

void PGNHandler::saveToFile(const Game& game, const std::string& filename) {
    std::ofstream outfile(filename);
    if (outfile.is_open()) {
        outfile << generatePGN(game);
        outfile.close();
    } else {
        std::cerr << "Unable to open file for saving: " << filename << std::endl;
    }
}

std::string PGNHandler::generatePGN(const Game& game) {
    std::stringstream ss;
    ss << "[Event \"Console Chess Game\"]\n";
    ss << "[Date \"????.??.??\"]\n"; 
    ss << "\n";

    std::vector<std::string> history = game.getMoveHistory();
    for (size_t i = 0; i < history.size(); ++i) {
        if (i % 2 == 0) {
            ss << (i / 2 + 1) << ". ";
        }
        ss << history[i] << " ";
    }
    return ss.str();
}

void PGNHandler::loadFromFile(Game& game, const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Unable to open file for loading: " << filename << std::endl;
        return;
    }

    // Reset game
    game = Game();

    std::string content((std::istreambuf_iterator<char>(infile)),
                        (std::istreambuf_iterator<char>()));
    infile.close();

    std::string cleanContent;
    bool inTag = false;
    bool inComment = false;
    
    // Simple state machine to remove tags and comments
    for (size_t i = 0; i < content.length(); ++i) {
        char c = content[i];
        
        if (inTag) {
            if (c == ']') inTag = false;
            continue;
        }
        if (inComment) {
            if (c == '}') inComment = false;
            continue;
        }
        
        if (c == '[') {
            inTag = true; 
            continue;
        }
        if (c == '{') {
            inComment = true; 
            continue;
        }
        if (c == ';') {
            // Skip rest of line
            while (i < content.length() && content[i] != '\n') {
                i++;
            }
            continue;
        }
        
        cleanContent += c;
    }
    
    std::stringstream ss(cleanContent);
    std::string token;
    while (ss >> token) {
        // Skip junk
        if (token.empty()) continue;

        // Skip move numbers (e.g., "1.", "25.")
        if (std::isdigit(token[0])) {
            bool isNumber = true;
            // It might be 1-0 result
            if (token == "1-0" || token == "0-1" || token == "1/2-1/2") {
                break; 
            }
            // If it ends with '.', treat as number label
            if (token.back() == '.') continue;
            
            // Checking if it's just digits (unlikely in PGN unless "1")
        }

        // Handle results like 1-0, 0-1, 1/2-1/2
        if (token == "1-0" || token == "0-1" || token == "1/2-1/2") {
            break;
        }
        
        // Parse move
        ParsedMove parsed = MoveParser::parse(token, game.getBoard(), game.getCurrentPlayer());
        if (parsed.isValid && parsed.move.has_value()) {
            if (!game.makeMove(parsed.move.value())) {
                 std::cerr << "Failed to apply move from PGN: " << token << std::endl;
                 // Don't break, maybe try next? Usually PGN is strict.
                 break;
            }
        } else {
             std::cerr << "Invalid move detected in PGN: " << token << std::endl;
             // Might happen for result tokens if missed above
        }
    }
}
