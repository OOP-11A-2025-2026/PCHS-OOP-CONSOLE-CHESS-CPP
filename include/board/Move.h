#ifndef MOVE_H
#define MOVE_H

#include "enums/PieceType.h"
#include "board/Square.h"
#include <optional>

/**
 * Represents a chess move from one square to another.
 * Optionally includes promotion information for pawn promotion moves.
 */
class Move {
private:
    Square from;
    Square to;
    std::optional<PieceType> promotion;

public:
    /**
     * Creates a new Move without promotion.
     * @param from The starting square
     * @param to The destination square
     */
    Move(const Square& from, const Square& to);

    /**
     * Creates a new Move with optional promotion.
     * @param from The starting square
     * @param to The destination square
     * @param promotion The piece type to promote to (nullopt if not a promotion)
     */
    Move(const Square& from, const Square& to, const std::optional<PieceType>& promotion);

    /**
     * Gets the starting square of the move.
     * @return The source square
     */
    Square getFrom() const;

    /**
     * Gets the destination square of the move.
     * @return The target square
     */
    Square getTo() const;

    /**
     * Gets the promotion piece type, if this is a pawn promotion move.
     * @return The piece type to promote to, or nullopt if not a promotion
     */
    std::optional<PieceType> getPromotion() const;

    /**
     * Checks if this move includes a promotion.
     * @return true if this is a promotion move, false otherwise
     */
    bool isPromotion() const;
};

#endif // MOVE_H