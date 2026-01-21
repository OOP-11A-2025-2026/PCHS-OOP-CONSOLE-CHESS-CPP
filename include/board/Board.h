#ifndef BOARD_H
#define BOARD_H

#include "board/Move.h"
#include "board/Square.h"
#include "pieces/Piece.h"
#include "enums/Color.h"
#include <vector>

class Board {
private:
    Piece* squares[8][8];
    std::vector<Piece*> whitePieces;
    std::vector<Piece*> blackPieces;
    Square enPassantTarget;
    bool enPassantAvailable;
    bool whiteKingMoved = false;
    bool blackKingMoved = false;
    bool whiteRookA_Moved = false;
    bool whiteRookH_Moved = false;
    bool blackRookA_Moved = false;
    bool blackRookH_Moved = false;
    Move* lastMove = nullptr;

    /**
     * Helper to check if a move is an en passant capture.
     */
    bool isEnPassantMove(const Move& move, const Piece* moving) const;

    /**
     * Helper to check if two squares are the same.
     */
    bool sameSquare(const Square& a, const Square& b) const;

    /**
     * Helper to check if a pawn has reached promotion rank.
     */
    bool isPawnPromotion(const Piece* piece, const Square& to) const;

public:
    Board();
    Board(const Board& other);
    Board& operator=(const Board& other);
    ~Board();

    bool isInside(int file, int rank) const;
    bool isEmpty(int file, int rank) const;
    bool isEnemy(int file, int rank, Color myColor) const;
    Piece* getPieceAt(int file, int rank) const;

    /**
     * Gets piece at the specified square.
     */
    Piece* getPieceAt(const Square& square) const;

    /**
     * Sets piece at the specified square.
     */
    void setPieceAt(const Square& square, Piece* piece);

    /**
     * Checks if a square is within board bounds.
     */
    bool isInBounds(const Square& square) const;

    /**
     * Checks if the piece at the square belongs to the specified color.
     */
    bool isOwnPiece(const Square& square, Color color) const;

    /**
     * Checks if the path between two squares is clear (no pieces blocking).
     * Works for straight lines (rook moves) and diagonals (bishop moves).
     */
    bool isPathClear(const Square& from, const Square& to) const;

    /**
     * Creates a deep copy (clone) of this board.
     */
    Board clone() const;

    /**
     * Gets the last move made on this board.
     */
    Move* getLastMove() const;

    /**
     * Applies a move to the board and returns the captured piece (if any).
     * Handles regular moves, captures, castling, en passant, and promotion.
     */
    Piece* applyMove(const Move& move);

    /**
     * Checks if a square is attacked by pieces of the specified color.
     * Tests all possible attacks: pawns, knights, king, and sliding pieces.
     */
    bool isSquareAttacked(const Square& target, Color byColor) const;

    /**
     * Simulates the given move on a clone of this board and detects whether
     * the moving side's king would be left in check.
     */
    bool simulateMoveAndDetectSelfCheck(const Move& move, Color movingColor) const;

    void placePiece(Piece* piece);
    void removePieceAt(int file, int rank);
    void makeMove(const Move& move);
    void undoMove(const Move& move, Piece* captured, Square from, Square to);

    bool isSquareAttacked(int file, int rank, Color byColor) const;

    std::vector<Piece*>& getPieces(Color color);
    const std::vector<Piece*>& getPieces(Color color) const;

    bool isLegalMove(const Move& move, Color turn) const;

    bool canCastleKingSide(Color turn) const;
    bool canCastleQueenSide(Color turn) const;
    bool isEnPassantAvailable() const;
    Square getEnPassantTarget() const;

    Move getCastlingMove(Color turn, bool kingSide) const;
    void performCastling(Color turn, bool kingSide);
};

#endif