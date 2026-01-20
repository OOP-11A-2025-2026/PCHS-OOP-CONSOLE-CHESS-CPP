#include "pieces/Bishop.h"
#include "board/Board.h"

Bishop::Bishop(Color color, int file, int rank)
    : Piece(color, PieceType::BISHOP, file, rank) {}

std::vector<Move> Bishop::getLegalMoves(const Board& board) const {
    std::vector<Move> moves;

    const int dirs[4][2] = {
        { 1, 1}, { 1,-1}, {-1, 1}, {-1,-1}
    };

    for (const auto& d : dirs) {
        int f = file + d[0];
        int r = rank + d[1];

        while (board.isInside(f, r)) {
            if (board.isEmpty(f, r)) {
                moves.emplace_back(Square(file, rank), Square(f, r));
            } else {
                if (board.isEnemy(f, r, color))
                    moves.emplace_back(Square(file, rank), Square(f, r));
                break;
            }
            f += d[0];
            r += d[1];
        }
    }

    return moves;
}

Piece* Bishop::clone() const {
    return new Bishop(*this);
}
