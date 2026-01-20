#include "pieces/Pawn.h"
#include "board/Board.h"


Pawn::Pawn(Color color, int file, int rank)
    : Piece(color, PieceType::PAWN, file, rank) {}

std::vector<Move> Pawn::getLegalMoves(const Board& board) const {
    std::vector<Move> moves;

    int dir = (color == Color::WHITE) ? 1 : -1;
    int startRank = (color == Color::WHITE) ? 1 : 6;

    if (board.isInside(file, rank + dir) &&
        board.isEmpty(file, rank + dir)) {

        moves.emplace_back(
            Square(file, rank),
            Square(file, rank + dir)
        );

        if (rank == startRank &&
            board.isEmpty(file, rank + 2 * dir)) {

            moves.emplace_back(
                Square(file, rank),
                Square(file, rank + 2 * dir)
            );
        }
    }

    for (int df : {-1, 1}) {
        int nf = file + df;
        int nr = rank + dir;

        if (board.isInside(nf, nr) &&
            board.isEnemy(nf, nr, color)) {

            moves.emplace_back(
                Square(file, rank),
                Square(nf, nr)
            );
        }
    }

    return moves;
}

Piece* Pawn::clone() const {
    return new Pawn(*this);
}
