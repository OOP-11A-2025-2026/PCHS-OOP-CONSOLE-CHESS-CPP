#include "pieces/Pawn.h"
#include "board/Board.h"

Pawn::Pawn(Color color, int file, int rank)
    : Piece(color, PieceType::PAWN, file, rank) {}

std::vector<Move> Pawn::getLegalMoves(const Board& board) const {
    std::vector<Move> moves;

    int dir = (color == Color::WHITE) ? 1 : -1;
    int startRank = (color == Color::WHITE) ? 1 : 6;
    int promotionRank = (color == Color::WHITE) ? 7 : 0;

    if (board.isInside(file, rank + dir) && board.isEmpty(file, rank + dir)) {
        if (rank + dir == promotionRank) {
            moves.emplace_back(Square(file, rank), Square(file, rank + dir), PieceType::QUEEN);
            moves.emplace_back(Square(file, rank), Square(file, rank + dir), PieceType::ROOK);
            moves.emplace_back(Square(file, rank), Square(file, rank + dir), PieceType::BISHOP);
            moves.emplace_back(Square(file, rank), Square(file, rank + dir), PieceType::KNIGHT);
        } else {
            moves.emplace_back(Square(file, rank), Square(file, rank + dir));
        }

        if (rank == startRank && board.isEmpty(file, rank + 2 * dir)) {
            moves.emplace_back(Square(file, rank), Square(file, rank + 2 * dir));
        }
    }

    for (int df : {-1, 1}) {
        int nf = file + df;
        int nr = rank + dir;

        if (board.isInside(nf, nr)) {
            if (board.isEnemy(nf, nr, color)) {
                if (nr == promotionRank) {
                    moves.emplace_back(Square(file, rank), Square(nf, nr), PieceType::QUEEN);
                    moves.emplace_back(Square(file, rank), Square(nf, nr), PieceType::ROOK);
                    moves.emplace_back(Square(file, rank), Square(nf, nr), PieceType::BISHOP);
                    moves.emplace_back(Square(file, rank), Square(nf, nr), PieceType::KNIGHT);
                } else {
                    moves.emplace_back(Square(file, rank), Square(nf, nr));
                }
            }

            if (board.isEnPassantAvailable() && board.getEnPassantTarget() == Square(nf, nr)) {
                moves.emplace_back(Square(file, rank), Square(nf, nr));
            }
        }
    }

    return moves;
}

Piece* Pawn::clone() const {
    return new Pawn(*this);
}
