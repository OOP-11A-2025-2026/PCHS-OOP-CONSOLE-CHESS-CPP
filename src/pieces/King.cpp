#include "pieces/King.h"
#include "board/Board.h"

King::King(Color color, int file, int rank)
    : Piece(color, PieceType::KING, file, rank) {}

std::vector<Move> King::getLegalMoves(const Board& board) const {
    std::vector<Move> moves;

    // Normal king moves
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

    // Castling
    if (color == Color::WHITE && file == 4 && rank == 0) {
        if (board.canCastleKingSide(Color::WHITE)) {
            moves.emplace_back(Square(4,0), Square(6,0));
        }
        if (board.canCastleQueenSide(Color::WHITE)) {
            moves.emplace_back(Square(4,0), Square(2,0));
        }
    }

    if (color == Color::BLACK && file == 4 && rank == 7) {
        if (board.canCastleKingSide(Color::BLACK)) {
            moves.emplace_back(Square(4,7), Square(6,7));
        }
        if (board.canCastleQueenSide(Color::BLACK)) {
            moves.emplace_back(Square(4,7), Square(2,7));
        }
    }

    return moves;
}

Piece* King::clone() const {
    return new King(*this);
}
