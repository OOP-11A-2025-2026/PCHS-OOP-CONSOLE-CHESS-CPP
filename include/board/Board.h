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

public:
    Board();
    Board(const Board& other);
    ~Board();

    bool isInside(int file, int rank) const;
    bool isEmpty(int file, int rank) const;
    bool isEnemy(int file, int rank, Color myColor) const;
    Piece* getPieceAt(int file, int rank) const;

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
