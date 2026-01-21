#ifndef CHESSCLI_H
#define CHESSCLI_H

#include <string>
#include "game/Game.h"
#include "timer/Timer.h"

/**
 * Main command-line interface for the chess game.
 * Handles menus, game loop, user input, and game flow.
 */
class ChessCLI {
private:
    Game* game;
    Timer* timer;

    /**
     * Displays the main menu and handles menu selection.
     */
    void showMainMenu();

    /**
     * Starts a new game with fresh board and timer.
     */
    void startNewGame();

    /**
     * Loads a game from a PGN file.
     * NOTE: This function requires PGNParser implementation.
     */
    void loadGame();

    /**
     * Main game loop - displays board, handles moves, checks game state.
     */
    void gameLoop();

    /**
     * Handles user commands and move input.
     * @param input The user's input string
     */
    void handleCommand(const std::string& input);

    /**
     * Handles a pending draw offer from opponent.
     */
    void handlePendingDrawOffer();

    /**
     * Prints whose turn it is to move.
     */
    void printTurnPrompt();

    /**
     * Prints the current game status (check, checkmate, stalemate, etc.).
     */
    void printGameStatus();

    /**
     * Checks if the game is over.
     * @return true if game is finished
     */
    bool isGameOver();

    /**
     * Prints available in-game commands.
     */
    void printInGameMenu();

    /**
     * Saves the current game to a PGN file.
     * NOTE: This function requires PGNExporter implementation.
     */
    void saveGame();

    // UI Helper functions
    void printBox(const std::string& text, int width);
    void printSeparator(int width);
    void printCentered(const std::string& text, int width);
    void printHighlight(const std::string& text, int width);
    void printMenuOption(const std::string& number, const std::string& label);
    void pause();
    void clearScreen();

    /**
     * Reads a line of input from the user.
     * @return The trimmed input string
     */
    std::string readLine();

public:
    /**
     * Constructs a new ChessCLI.
     */
    ChessCLI();

    /**
     * Destructor - cleans up game and timer.
     */
    ~ChessCLI();

    /**
     * Starts the chess application.
     */
    void start();
};

#endif // CHESSCLI_H