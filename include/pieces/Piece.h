#pragma once

#include <vector>
#include "board/Move.h"
#include "enums/Color.h"
#include "enums/PieceType.h"

class Board; // forward declaration

/**
 * Abstract base class for all chess pieces.
 * Defines common properties and methods that all pieces must implement.
 */
class Piece {
protected:
    const Color color;   // WHITE or BLACK
    PieceType type;
    int file;  // 0–7 for a–h
    int rank;  // 0–7 for 1–8

public:
    /**
     * Constructs a piece with the specified color, type, and position.
     * @param color The color of the piece (WHITE or BLACK)
     * @param type The type of the piece
     * @param file The file (column) position (0-7)
     * @param rank The rank (row) position (0-7)
     */
    Piece(Color color, PieceType type, int file, int rank);
    
    virtual ~Piece() = default;

    // Getters
    /**
     * Gets the color of this piece.
     * @return The piece color
     */
    Color getColor() const;

    /**
     * Gets the type of this piece.
     * @return The piece type
     */
    PieceType getType() const;

    /**
     * Gets the file (column) position of this piece.
     * @return File index (0-7)
     */
    int getFile() const;

    /**
     * Gets the rank (row) position of this piece.
     * @return Rank index (0-7)
     */
    int getRank() const;

    /**
     * Sets the position of this piece.
     * @param file The new file (column) position
     * @param rank The new rank (row) position
     */
    void setPosition(int file, int rank);

    /**
     * Gets all legal moves for this piece given the current board state.
     * Must be implemented by derived classes.
     * @param board The current board state
     * @return Vector of legal moves
     */
    virtual std::vector<Move> getLegalMoves(const Board& board) const = 0;

    /**
     * Creates a deep copy of this piece.
     * Must be implemented by derived classes.
     * @return Pointer to a new piece that is a copy of this one
     */
    virtual Piece* clone() const = 0;
};