#ifndef KING_H
#define KING_H

#include "pieces/Piece.h"

class King : public Piece {
public:
    King(Color color, int file, int rank);

    std::vector<Move> getLegalMoves(const Board& board) const override;
    Piece* clone() const override;
};

#endif
