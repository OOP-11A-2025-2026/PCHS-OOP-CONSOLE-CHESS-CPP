#include "pieces/King.h"
#include "board/Board.h"

King::King(Color color, int file, int rank)
    : Piece(color, PieceType::KING, file, rank) {}

std::vector<Move> King::getLegalMoves(const Board& board) const {
    std::vector<Move> moves;

    for (int df = -1; df <= 1; df++) {
        for (int dr = -1; dr <= 1; dr++) {
            if (df == 0 && dr == 0) continue;

            int nf = file + df;
            int nr = rank + dr;

            if (!board.isInside(nf, nr)) continue;

            if (board.isEmpty(nf, nr) ||
                board.isEnemy(nf, nr, color)) {

                moves.emplace_back(
                    Square(file, rank),
                    Square(nf, nr)
                );
            }
        }
    }
    return moves;
}

Piece* King::clone() const {
    return new King(*this);
}
