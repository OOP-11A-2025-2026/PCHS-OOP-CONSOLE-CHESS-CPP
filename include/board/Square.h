#ifndef SQUARE_H
#define SQUARE_H

#include <string>

/**
 * Represents a square on the chess board.
 * Uses file (column a-h = 0-7) and rank (row 1-8 = 0-7) coordinates.
 */
class Square {
private:
    int file;  // 0–7 for a–h
    int rank;  // 0–7 for 1–8

public:
    /**
     * Creates a new Square with the specified file and rank.
     * @param file The file (column) index, 0-7 corresponding to a-h
     * @param rank The rank (row) index, 0-7 corresponding to 1-8
     */
    Square(int file, int rank);

    /**
     * Gets the file (column) index of this square.
     * @return File index (0-7 for a-h)
     */
    int getFile() const;

    /**
     * Gets the rank (row) index of this square.
     * @return Rank index (0-7 for 1-8)
     */
    int getRank() const;

    /**
     * Parses a square from algebraic notation string (e.g., "e4", "a1").
     * @param s The algebraic notation string (2 characters: file letter + rank number)
     * @param result Reference to Square object to store the result
     * @return true if parsing was successful, false otherwise
     */
    static bool fromString(const std::string& s, Square& result);

    /**
     * Converts this square to algebraic notation string.
     * @return String representation (e.g., "e4", "a1")
     */
    std::string toString() const;

    /**
     * Checks equality with another square.
     * @param other Square to compare with
     * @return true if both squares have the same file and rank
     */
    bool operator==(const Square& other) const;

    /**
     * Checks inequality with another square.
     * @param other Square to compare with
     * @return true if squares have different file or rank
     */
    bool operator!=(const Square& other) const;

    /**
     * Generates a hash code for this square.
     * @return Hash code based on file and rank
     */
    size_t hashCode() const;
};

// Hash function for using Square in unordered containers
namespace std {
    template<>
    struct hash<Square> {
        size_t operator()(const Square& square) const {
            return square.hashCode();
        }
    };
}

#endif // SQUARE_H