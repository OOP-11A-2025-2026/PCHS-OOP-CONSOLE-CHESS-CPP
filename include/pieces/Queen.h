#ifndef QUEEN_H
#define QUEEN_H

#include "pieces/Piece.h"

class Queen : public Piece {
public:
    Queen(Color color, int file, int rank);

    std::vector<Move> getLegalMoves(const Board& board) const override;
    Piece* clone() const override;
};

#endif

