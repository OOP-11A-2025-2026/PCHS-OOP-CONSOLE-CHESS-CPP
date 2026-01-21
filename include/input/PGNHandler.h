#pragma once

#include <string>
#include "game/Game.h"

class PGNHandler {
public:
    /**
     * Saves the current game state to a PGN file.
     * @param game The game to save
     * @param filename The path to the file
     */
    static void saveToFile(const Game& game, const std::string& filename);

    /**
     * Loads a game from a PGN file.
     * Resets the provided game object and applies the moves from the file.
     * @param game The game object to update
     * @param filename The path to the file
     */
    static void loadFromFile(Game& game, const std::string& filename);

private:
    static std::string generatePGN(const Game& game);
};
