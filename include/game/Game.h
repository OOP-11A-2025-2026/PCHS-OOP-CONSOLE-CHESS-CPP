#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include <optional>
#include "board/Board.h"
#include "board/Move.h"
#include "board/Square.h"
#include "enums/Color.h"
#include "enums/PieceType.h"

class Piece;

/**
 * Enumeration of possible game states.
 */
enum class GameState {
    ONGOING,
    CHECK,
    CHECKMATE,
    STALEMATE,
    DRAW,
    RESIGNED
};

/**
 * Main game controller that manages the chess game state, move validation,
 * and game flow including check, checkmate, stalemate, and draw conditions.
 */
class Game {
private:
    Board board;
    Color currentPlayer;
    GameState state;
    bool drawOffered;
    std::optional<Color> drawOfferedBy;
    std::vector<std::string> moveHistory;

    /**
     * Initializes the chess board with pieces in starting positions.
     */
    void initializeBoard();

    /**
     * Updates the game state (check, checkmate, stalemate, ongoing).
     */
    void updateGameState();

    /**
     * Checks if the specified color has any legal moves available.
     * @param color The color to check
     * @return true if there are legal moves, false otherwise
     */
    bool hasAnyLegalMoves(Color color);

    /**
     * Finds the king's square for the specified color.
     * @param board The board to search
     * @param color The color of the king to find
     * @return The square containing the king, or std::nullopt if not found
     */
    std::optional<Square> findKing(const Board& board, Color color) const;

    /**
     * Gets the opponent's color.
     * @param color The current player's color
     * @return The opponent's color
     */
    Color getOpponentColor(Color color) const;

    /**
     * Converts a move to Standard Algebraic Notation (SAN).
     * @param move The move to convert
     * @param piece The piece being moved
     * @return The move in SAN notation
     */
    std::string moveToSAN(const Move& move, const Piece* piece);

public:
    /**
     * Constructs a new game with the board in starting position.
     */
    Game();

    /**
     * Gets the current game board.
     * @return Reference to the board
     */
    Board& getBoard();

    /**
     * Gets the current game board (const version).
     * @return Const reference to the board
     */
    const Board& getBoard() const;

    /**
     * Gets the current player's color.
     * @return The current player's color
     */
    Color getCurrentPlayer() const;

    /**
     * Sets the current player.
     * @param color The color to set as current player
     */
    void setCurrentPlayer(Color color);

    /**
     * Gets the current game state.
     * @return The current game state
     */
    GameState getState() const;

    /**
     * Checks if a draw has been offered.
     * @return true if a draw is currently offered
     */
    bool isDrawOffered() const;

    /**
     * Gets the color that offered the draw.
     * @return The color that offered the draw, or std::nullopt if no draw offered
     */
    std::optional<Color> getDrawOfferedBy() const;

    /**
     * Current player offers a draw.
     */
    void offerDraw();

    /**
     * Current player accepts the offered draw.
     */
    void acceptDraw();

    /**
     * Current player declines the offered draw.
     */
    void declineDraw();

    /**
     * Current player resigns the game.
     */
    void resign();

    /**
     * Attempts to make a move on the board.
     * Validates the move is legal and updates game state accordingly.
     * @param move The move to make
     * @return true if the move was successful, false if illegal
     */
    bool makeMove(const Move& move);

    /**
     * Gets the winner of the game.
     * @return "White", "Black", or empty string if no winner yet
     */
    std::string getWinner() const;

    /**
     * Gets the move history in SAN notation.
     * @return Vector of moves in SAN notation
     */
    std::vector<std::string> getMoveHistory() const;

    /**
     * Sets the move history.
     * @param history Vector of moves in SAN notation
     */
    void setMoveHistory(const std::vector<std::string>& history);
};

#endif // GAME_H