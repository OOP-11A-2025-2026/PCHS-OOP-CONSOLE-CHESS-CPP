#pragma once
#include <optional>

/**
 * Enumeration of all chess piece types with their standard algebraic notation symbols.
 * K=King, Q=Queen, R=Rook, B=Bishop, N=Knight, P=Pawn
 */
enum class PieceType {
    KING,
    QUEEN,
    ROOK,
    BISHOP,
    KNIGHT,
    PAWN
};

/**
 * Returns the algebraic notation symbol for a piece type.
 * @param type The PieceType
 * @return Character symbol ('K', 'Q', 'R', 'B', 'N', 'P')
 */
inline char pieceTypeToChar(PieceType type) {
    switch (type) {
        case PieceType::KING:   return 'K';
        case PieceType::QUEEN:  return 'Q';
        case PieceType::ROOK:   return 'R';
        case PieceType::BISHOP: return 'B';
        case PieceType::KNIGHT: return 'N';
        case PieceType::PAWN:   return 'P';
    }
    return '?'; // fallback (should not happen)
}

/**
 * Converts a character to the corresponding PieceType.
 * @param c Character ('K', 'Q', 'R', 'B', 'N', 'P')
 * @return Optional containing PieceType if valid, empty otherwise
 */
inline std::optional<PieceType> charToPieceType(char c) {
    switch (c) {
        case 'K': return PieceType::KING;
        case 'Q': return PieceType::QUEEN;
        case 'R': return PieceType::ROOK;
        case 'B': return PieceType::BISHOP;
        case 'N': return PieceType::KNIGHT;
        case 'P': return PieceType::PAWN;
        default:  return std::nullopt;
    }
}
