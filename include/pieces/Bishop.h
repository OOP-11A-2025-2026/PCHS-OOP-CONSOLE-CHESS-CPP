#ifndef BISHOP_H
#define BISHOP_H

#include "pieces/Piece.h"

class Bishop : public Piece {
public:
    Bishop(Color color, int file, int rank);

    std::vector<Move> getLegalMoves(const Board& board) const override;
    Piece* clone() const override;
};

#endif
