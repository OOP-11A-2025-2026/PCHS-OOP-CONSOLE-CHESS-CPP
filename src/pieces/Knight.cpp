#include "pieces/Knight.h"
#include "board/Board.h"

Knight::Knight(Color color, int file, int rank)
    : Piece(color, PieceType::KNIGHT, file, rank) {}

std::vector<Move> Knight::getLegalMoves(const Board& board) const {
    std::vector<Move> moves;

    const int jumps[8][2] = {
        { 1, 2}, { 2, 1}, {-1, 2}, {-2, 1},
        { 1,-2}, { 2,-1}, {-1,-2}, {-2,-1}
    };

    for (const auto& j : jumps) {
        int nf = file + j[0];
        int nr = rank + j[1];

        if (!board.isInside(nf, nr)) continue;

        if (board.isEmpty(nf, nr) ||
            board.isEnemy(nf, nr, color)) {

            moves.emplace_back(
                Square(file, rank),
                Square(nf, nr)
            );
        }
    }

    return moves;
}

Piece* Knight::clone() const {
    return new Knight(*this);
}
