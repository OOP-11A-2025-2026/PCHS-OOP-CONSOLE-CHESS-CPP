#include "board/Square.h"

/**
 * Creates a new Square with the specified file and rank.
 */
Square::Square(int file, int rank) : file(file), rank(rank) {}

/**
 * Gets the file (column) index of this square.
 */
int Square::getFile() const {
    return file;
}

/**
 * Gets the rank (row) index of this square.
 */
int Square::getRank() const {
    return rank;
}

/**
 * Parses a square from algebraic notation string (e.g., "e4", "a1").
 */
bool Square::fromString(const std::string& s, Square& result) {
    if (s.length() != 2) {
        return false;
    }

    char f = s[0]; // 'a'..'h'
    char r = s[1]; // '1'..'8'
    
    int file = f - 'a';
    int rank = r - '1';
    
    if (file < 0 || file > 7 || rank < 0 || rank > 7) {
        return false;
    }
    
    result = Square(file, rank);
    return true;
}

/**
 * Converts this square to algebraic notation string.
 */
std::string Square::toString() const {
    char fileChar = static_cast<char>('a' + file);
    int rankNum = rank + 1;
    return std::string(1, fileChar) + std::to_string(rankNum);
}

/**
 * Checks equality with another square.
 */
bool Square::operator==(const Square& other) const {
    return file == other.file && rank == other.rank;
}

/**
 * Checks inequality with another square.
 */
bool Square::operator!=(const Square& other) const {
    return !(*this == other);
}

/**
 * Generates a hash code for this square.
 */
size_t Square::hashCode() const {
    return 31 * file + rank;
}