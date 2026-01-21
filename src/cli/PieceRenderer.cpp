#include "cli/PieceRenderer.h"
#include "pieces/Piece.h"
#include "enums/Color.h"
#include "enums/PieceType.h"

/**
 * Converts a piece to its Unicode chess symbol.
 */
std::string PieceRenderer::toSymbol(const Piece* piece) {
    if (piece == nullptr) {
        return " ";
    }

    switch (piece->getType()) {
        case PieceType::KING:
            return piece->getColor() == Color::WHITE ? "♔" : "♚";
        case PieceType::QUEEN:
            return piece->getColor() == Color::WHITE ? "♕" : "♛";
        case PieceType::ROOK:
            return piece->getColor() == Color::WHITE ? "♖" : "♜";
        case PieceType::BISHOP:
            return piece->getColor() == Color::WHITE ? "♗" : "♝";
        case PieceType::KNIGHT:
            return piece->getColor() == Color::WHITE ? "♘" : "♞";
        case PieceType::PAWN:
            return piece->getColor() == Color::WHITE ? "♙" : "♟";
        default:
            return "?";
    }
}