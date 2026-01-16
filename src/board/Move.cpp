#include "board/Move.h"

/**
 * Creates a new Move without promotion.
 */
Move::Move(const Square& from, const Square& to) 
    : from(from), to(to), promotion(std::nullopt) {}

/**
 * Creates a new Move with optional promotion.
 */
Move::Move(const Square& from, const Square& to, const std::optional<PieceType>& promotion)
    : from(from), to(to), promotion(promotion) {}

/**
 * Gets the starting square of the move.
 */
Square Move::getFrom() const {
    return from;
}

/**
 * Gets the destination square of the move.
 */
Square Move::getTo() const {
    return to;
}

/**
 * Gets the promotion piece type, if this is a pawn promotion move.
 */
std::optional<PieceType> Move::getPromotion() const {
    return promotion;
}

/**
 * Checks if this move includes a promotion.
 */
bool Move::isPromotion() const {
    return promotion.has_value();
}