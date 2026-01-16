#include "pieces/Piece.h"

/**
 * Constructs a piece with the specified color, type, and position.
 */
Piece::Piece(Color color, PieceType type, int file, int rank)
    : color(color), type(type), file(file), rank(rank) {}

/**
 * Gets the color of this piece.
 */
Color Piece::getColor() const {
    return color;
}

/**
 * Gets the type of this piece.
 */
PieceType Piece::getType() const {
    return type;
}

/**
 * Gets the file (column) position of this piece.
 */
int Piece::getFile() const {
    return file;
}

/**
 * Gets the rank (row) position of this piece.
 */
int Piece::getRank() const {
    return rank;
}

/**
 * Sets the position of this piece.
 */
void Piece::setPosition(int newFile, int newRank) {
    file = newFile;
    rank = newRank;
}