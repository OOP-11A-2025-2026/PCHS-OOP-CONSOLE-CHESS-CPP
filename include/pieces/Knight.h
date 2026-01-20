#ifndef KNIGHT_H
#define KNIGHT_H

#include "pieces/Piece.h"

class Knight : public Piece {
public:
    Knight(Color color, int file, int rank);

    std::vector<Move> getLegalMoves(const Board& board) const override;
    Piece* clone() const override;
};

#endif
