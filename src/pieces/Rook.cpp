#include "pieces/Rook.h"
#include "board/Board.h"

Rook::Rook(Color color, int file, int rank)
    : Piece(color, PieceType::ROOK, file, rank) {}

std::vector<Move> Rook::getLegalMoves(const Board& board) const {
    std::vector<Move> moves;

    const int dirs[4][2] = {
        { 1, 0}, {-1, 0}, { 0, 1}, { 0,-1}
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

Piece* Rook::clone() const {
    return new Rook(*this);
}
