#pragma once

#include <vector>
#include "pieces/Piece.h"
#include "board/Board.h"
#include "enums/Color.h"

class Rook : public Piece {
public:
    Rook(Color color, int file, int rank);
    std::vector<Move> getLegalMoves(const Board& board) const override;
    Piece* clone() const override;
};
