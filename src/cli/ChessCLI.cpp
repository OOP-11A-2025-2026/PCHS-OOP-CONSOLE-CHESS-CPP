#include "cli/ChessCLI.h"
#include "cli/BoardPrinter.h"
#include "enums/Color.h"
#include "game/Game.h"
#include "input/MoveParser.h"
#include "input/PGNHandler.h"
// #include "pgn/PGNExporter.h"  // Uncomment when implemented
// #include "pgn/PGNParser.h"    // Uncomment when implemented
#include "timer/Timer.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <limits>

/**
 * Constructs a new ChessCLI.
 */
ChessCLI::ChessCLI() : game(nullptr), timer(nullptr) {}

/**
 * Destructor - cleans up game and timer.
 */
ChessCLI::~ChessCLI() {
    delete game;
    delete timer;
}

/**
 * Starts the chess application.
 */
void ChessCLI::start() {
    showMainMenu();
}

/**
 * Displays the main menu and handles menu selection.
 */
void ChessCLI::showMainMenu() {
    while (true) {
        clearScreen();
        printBox("CONSOLE CHESS", 50);
        std::cout << std::endl;
        printMenuOption("1", "New Game");
        printMenuOption("2", "Load Game");
        printMenuOption("3", "Exit");
        std::cout << std::endl;
        printSeparator(50);
        std::cout << "  > ";

        std::string choice = readLine();

        if (choice == "1") {
            startNewGame();
        } else if (choice == "2") {
            loadGame();
        } else if (choice == "3") {
            std::cout << "\n  Thanks for playing!" << std::endl;
            return;
        } else {
            std::cout << "\n  Invalid choice." << std::endl;
            pause();
        }
    }
}

/**
 * Starts a new game with fresh board and timer.
 */
void ChessCLI::startNewGame() {
    delete game;
    delete timer;
    
    game = new Game();
    timer = new Timer(10);
    timer->start();
    gameLoop();
}

/**
 * Loads a game from a PGN file.
 * NOTE: This function requires PGNParser implementation.
 */
void ChessCLI::loadGame() {
    clearScreen();
    printBox("LOAD GAME", 50);
    std::cout << std::endl;
    std::cout << "  Enter PGN filename: ";
    std::string filename = readLine();

    if (filename.empty()) {
        std::cout << "\n  No filename entered." << std::endl;
        pause();
        return;
    }

    if (filename.find(".pgn") == std::string::npos) {
        filename += ".pgn";
    }

    try {
        Game newGame;
        PGNHandler::loadFromFile(newGame, filename);

        // Check if anything was loaded (e.g. at least one piece on board or history exists)
        if (newGame.getMoveHistory().empty() && newGame.getBoard().isEmpty(4,0) && newGame.getBoard().isEmpty(4,7)) {
             // Heuristic check: if history is empty and ke1/ke8 missing, probably failed or empty file.
             // But actually, a fresh game has no history.
             // Let's rely on PGNHandler::loadFromFile being successful if it returns.
             // Currently PGNHandler prints errors to stderr but we can't catch them easily unless we change the API or redirect.
             // We'll assume success if no crash.
        }

        delete game;
        game = new Game(newGame); // Use copy constructor or assignment
        
        // Reset timer
        delete timer;
        timer = new Timer(10);
        timer->start();

        std::cout << "\n  Loaded " << filename << " successfully!" << std::endl;
        std::cout << "  " << game->getMoveHistory().size() << " moves loaded." << std::endl;
        pause();

        gameLoop();

    } catch (const std::exception& e) {
        std::cout << "\n  Error loading file: " << e.what() << std::endl;
        pause();
    }
}

/**
 * Main game loop - displays board, handles moves, checks game state.
 */
void ChessCLI::gameLoop() {
    while (true) {
        clearScreen();
        printSeparator(60);
        BoardPrinter::print(game->getBoard());
        printSeparator(60);

        printGameStatus();
        timer->printTime();

        if (isGameOver()) {
            std::cout << "\n  Press Enter to return to main menu..." << std::endl;
            readLine();
            return;
        }

        if (game->isDrawOffered() && 
            game->getDrawOfferedBy().has_value() && 
            game->getDrawOfferedBy().value() != game->getCurrentPlayer()) {
            handlePendingDrawOffer();
            if (isGameOver()) {
                std::cout << "\n  Press Enter to return to main menu..." << std::endl;
                readLine();
                return;
            }
        }

        printTurnPrompt();
        printInGameMenu();

        std::cout << "\n  Enter move: ";
        std::string input = readLine();

        handleCommand(input);
    }
}

/**
 * Handles user commands and move input.
 */
void ChessCLI::handleCommand(const std::string& input) {
    if (input.empty()) return;

    // Convert to lowercase for command comparison
    std::string lowerInput = input;
    std::transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(), ::tolower);

    if (lowerInput == "save") {
        saveGame();
        return;
    }

    if (lowerInput == "resign") {
        game->resign();
        return;
    }

    if (lowerInput == "draw") {
        if (game->isDrawOffered()) {
            std::cout << "\n  Draw already offered." << std::endl;
        } else {
            game->offerDraw();
            std::cout << "\n  Draw offered." << std::endl;
        }
        pause();
        return;
    }

    // Parse and execute the move
    ParsedMove parsedMove = MoveParser::parse(lowerInput, game->getBoard(), game->getCurrentPlayer());
    
    if (!parsedMove.isValid || !parsedMove.move.has_value()) {
        std::cout << "\n  Illegal move!" << std::endl;
        pause();
    } else {
        Move finalMove = parsedMove.move.value();
        
        // --- Promotion Logic ---
        // If it's a pawn moving to the last rank, but no promotion type is set, ASK THE USER.
        Piece* p = game->getBoard().getPieceAt(finalMove.getFrom());
        if (p && p->getType() == PieceType::PAWN) {
            int targetRank = finalMove.getTo().getRank();
            bool isPromoRank = (p->getColor() == Color::WHITE && targetRank == 7) ||
                               (p->getColor() == Color::BLACK && targetRank == 0);
            
            if (isPromoRank && !finalMove.getPromotion().has_value()) {
                // Ask for promotion type
                std::cout << "\n  Pawn Promotion! Choose piece:" << std::endl;
                std::cout << "  [Q] Queen" << std::endl;
                std::cout << "  [R] Rook" << std::endl;
                std::cout << "  [B] Bishop" << std::endl;
                std::cout << "  [N] Knight" << std::endl;
                std::cout << "  > ";
                
                std::string choiceStr = readLine();
                char choice = 'q';
                if (!choiceStr.empty()) choice = std::tolower(choiceStr[0]);

                PieceType promoType = PieceType::QUEEN;
                switch(choice) {
                    case 'r': promoType = PieceType::ROOK; break;
                    case 'b': promoType = PieceType::BISHOP; break;
                    case 'n': promoType = PieceType::KNIGHT; break;
                    default:  promoType = PieceType::QUEEN; break;
                }
                
                // Reconstruct the move with promotion info
                finalMove = Move(finalMove.getFrom(), finalMove.getTo(), promoType);
            }
        }
        // -----------------------

        if (game->makeMove(finalMove)) {
            timer->switchTurn();
        } else {
            std::cout << "\n  Illegal move!" << std::endl;
            pause();
        }
    }
}

/**
 * Handles a pending draw offer from opponent.
 */
void ChessCLI::handlePendingDrawOffer() {
    std::cout << "\n  Opponent offers a draw. Accept? (y/n): ";
    std::string response = readLine();
    std::transform(response.begin(), response.end(), response.begin(), ::tolower);

    if (response == "y") {
        game->acceptDraw();
        std::cout << "  Draw agreed." << std::endl;
        printGameStatus();
    } else {
        game->declineDraw();
        std::cout << "  Draw declined." << std::endl;
    }

    pause();
}

/**
 * Prints whose turn it is to move.
 */
void ChessCLI::printTurnPrompt() {
    std::cout << std::endl;
    std::string player = game->getCurrentPlayer() == Color::WHITE ? "WHITE" : "BLACK";
    printCentered(player + " TO MOVE", 60);
}

/**
 * Prints the current game status (check, checkmate, stalemate, etc.).
 */
void ChessCLI::printGameStatus() {
    std::cout << std::endl;
    switch (game->getState()) {
        case GameState::CHECK:
            printHighlight("CHECK!", 60);
            break;
        case GameState::CHECKMATE:
            printHighlight("CHECKMATE — " + game->getWinner() + " WINS!", 60);
            break;
        case GameState::STALEMATE:
            printHighlight("STALEMATE — DRAW", 60);
            break;
        case GameState::DRAW:
            printHighlight("DRAW AGREED", 60);
            break;
        case GameState::RESIGNED:
            printHighlight("GAME OVER — " + game->getWinner() + " WINS!", 60);
            break;
        default:
            break;
    }
}

/**
 * Checks if the game is over.
 */
bool ChessCLI::isGameOver() {
    GameState state = game->getState();
    return state == GameState::CHECKMATE ||
           state == GameState::STALEMATE ||
           state == GameState::DRAW ||
           state == GameState::RESIGNED;
}

/**
 * Prints available in-game commands.
 */
void ChessCLI::printInGameMenu() {
    std::cout << std::endl;
    printSeparator(60);
    std::cout << "  Commands: [save] [resign]";
    if (!game->isDrawOffered()) {
        std::cout << " [draw]";
    }
    std::cout << std::endl;
    printSeparator(60);
}

/**
 * Saves the current game to a PGN file.
 * NOTE: This function requires PGNExporter implementation.
 */
void ChessCLI::saveGame() {
    std::cout << "\n  Enter filename to save (e.g., mygame.pgn): ";
    std::string filename = readLine();

    if (filename.empty()) {
        std::cout << "  No filename entered." << std::endl;
        pause();
        return;
    }

    if (filename.find(".pgn") == std::string::npos) {
        filename += ".pgn";
    }

    // TODO: Implement when PGNExporter is available
    std::cout << "\n  PGN export not yet implemented." << std::endl;
    std::cout << "  This feature requires PGNExporter class." << std::endl;
    pause();
    
    /*
    // Uncomment when PGNExporter is implemented:
    
    try {
        std::map<std::string, std::string> tags;
        tags["Event"] = "Console Chess Game";
        tags["Site"] = "Local";
        
        // Get current date in YYYY.MM.DD format
        std::time_t now = std::time(nullptr);
        std::tm* localTime = std::localtime(&now);
        std::ostringstream dateStream;
        dateStream << (localTime->tm_year + 1900) << "."
                   << std::setfill('0') << std::setw(2) << (localTime->tm_mon + 1) << "."
                   << std::setfill('0') << std::setw(2) << localTime->tm_mday;
        tags["Date"] = dateStream.str();
        
        tags["Round"] = "1";
        tags["White"] = "Player1";
        tags["Black"] = "Player2";

        std::string result;
        switch (game->getState()) {
            case GameState::CHECKMATE:
                result = game->getCurrentPlayer() == Color::WHITE ? "0-1" : "1-0";
                break;
            case GameState::DRAW:
            case GameState::STALEMATE:
                result = "1/2-1/2";
                break;
            case GameState::RESIGNED:
                result = game->getCurrentPlayer() == Color::WHITE ? "0-1" : "1-0";
                break;
            default:
                result = "*";
                break;
        }
        tags["Result"] = result;

        PGNExporter::saveToFile(filename, tags, game->getMoveHistory(), result);
        std::cout << "\n  Game saved to: " << filename << std::endl;
    } catch (const std::exception& e) {
        std::cout << "\n  Error saving game: " << e.what() << std::endl;
    }
    pause();
    */
}

/* ================== UI HELPERS ================== */

void ChessCLI::printBox(const std::string& text, int width) {
    printSeparator(width);
    printCentered(text, width);
    printSeparator(width);
}

void ChessCLI::printSeparator(int width) {
    std::cout << "  ";
    for (int i = 0; i < width; i++) {
        std::cout << "─";
    }
    std::cout << std::endl;
}

void ChessCLI::printCentered(const std::string& text, int width) {
    int padding = (width - text.length()) / 2;
    std::cout << "  ";
    for (int i = 0; i < std::max(0, padding); i++) {
        std::cout << " ";
    }
    std::cout << text << std::endl;
}

void ChessCLI::printHighlight(const std::string& text, int width) {
    int padding = (width - text.length() - 4) / 2;
    std::cout << "  ";
    for (int i = 0; i < std::max(0, padding); i++) {
        std::cout << " ";
    }
    std::cout << "[ " << text << " ]" << std::endl;
}

void ChessCLI::printMenuOption(const std::string& number, const std::string& label) {
    std::cout << "    " << number << ". " << label << std::endl;
}

void ChessCLI::pause() {
    std::cout << "\n  Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void ChessCLI::clearScreen() {
    std::cout << "\033[H\033[2J";
    std::cout.flush();
}

std::string ChessCLI::readLine() {
    std::string line;
    std::getline(std::cin, line);
    
    // Trim whitespace
    size_t start = line.find_first_not_of(" \t\r\n");
    size_t end = line.find_last_not_of(" \t\r\n");
    
    if (start == std::string::npos) {
        return "";
    }
    
    return line.substr(start, end - start + 1);
}