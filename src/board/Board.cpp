#include "board/Board.h"

Board::Board() {
    for (int f = 0; f < 8; f++) {
        for (int r = 0; r < 8; r++) {
            squares[f][r] = nullptr;
        }
    }
}

Board::~Board() {
    for (int f = 0; f < 8; f++) {
        for (int r = 0; r < 8; r++) {
            delete squares[f][r];
            squares[f][r] = nullptr;
        }
    }
}

bool Board::isInside(int file, int rank) const {
    return file >= 0 && file < 8 &&
           rank >= 0 && rank < 8;
}

Piece* Board::getPieceAt(int file, int rank) const {
    if (!isInside(file, rank)) return nullptr;
    return squares[file][rank];
}

bool Board::isEmpty(int file, int rank) const {
    return getPieceAt(file, rank) == nullptr;
}

bool Board::isEnemy(int file, int rank, Color myColor) const {
    Piece* p = getPieceAt(file, rank);
    return p != nullptr && p->getColor() != myColor;
}

void Board::placePiece(Piece* piece) {
    squares[piece->getFile()][piece->getRank()] = piece;
}

void Board::removePieceAt(int file, int rank) {
    if (!isInside(file, rank)) return;

    delete squares[file][rank];
    squares[file][rank] = nullptr;
}

void Board::makeMove(const Move& move) {
    Square from = move.getFrom();
    Square to = move.getTo();

    Piece* piece = getPieceAt(from.getFile(), from.getRank());
    if (!piece) return;

    removePieceAt(to.getFile(), to.getRank());

    squares[from.getFile()][from.getRank()] = nullptr;

    piece->setPosition(to.getFile(), to.getRank());
    squares[to.getFile()][to.getRank()] = piece;
}

bool Board::isSquareAttacked(int file, int rank, Color byColor) const {
    for (int f = 0; f < 8; f++) {
        for (int r = 0; r < 8; r++) {
            Piece* p = squares[f][r];
            if (!p || p->getColor() != byColor) continue;

            std::vector<Move> moves = p->getLegalMoves(*this);
            for (const Move& m : moves) {
                Square to = m.getTo();
                if (to.getFile() == file && to.getRank() == rank) {
                    return true;
                }
            }
        }
    }
    return false;
}
