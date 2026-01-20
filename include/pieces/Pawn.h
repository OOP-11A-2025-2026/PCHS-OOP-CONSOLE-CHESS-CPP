#pragma once
#include "pieces/Piece.h"

class Pawn : public Piece {
public:
    Pawn(Color color, int file, int rank);

    std::vector<Move> getLegalMoves(const Board& board) const override;
    Piece* clone() const override;
};
