#include "board/Board.h"
#include "board/Square.h"
#include "pieces/King.h"
#include "pieces/Rook.h"
#include <algorithm>

Board::Board() {
    for (int f = 0; f < 8; f++) {
        for (int r = 0; r < 8; r++) {
            squares[f][r] = nullptr;
        }
    }
}

Board::Board(const Board& other) {
    // Copy flags
    whiteKingMoved = other.whiteKingMoved;
    blackKingMoved = other.blackKingMoved;
    whiteRookA_Moved = other.whiteRookA_Moved;
    whiteRookH_Moved = other.whiteRookH_Moved;
    blackRookA_Moved = other.blackRookA_Moved;
    blackRookH_Moved = other.blackRookH_Moved;

    // Deep copy pieces
    for (int f = 0; f < 8; f++) {
        for (int r = 0; r < 8; r++) {
            if (other.squares[f][r]) {
                squares[f][r] = other.squares[f][r]->clone();
                Color c = squares[f][r]->getColor();
                if (c == Color::WHITE) whitePieces.push_back(squares[f][r]);
                else blackPieces.push_back(squares[f][r]);
            } else {
                squares[f][r] = nullptr;
            }
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
    if (piece->getColor() == Color::WHITE) whitePieces.push_back(piece);
    else blackPieces.push_back(piece);
}

void Board::removePieceAt(int file, int rank) {
    if (!isInside(file, rank)) return;
    Piece* piece = squares[file][rank];
    if (!piece) return;

    // Remove from vector
    auto& vec = (piece->getColor() == Color::WHITE) ? whitePieces : blackPieces;
    auto it = std::find(vec.begin(), vec.end(), piece);
    if (it != vec.end()) vec.erase(it);

    delete squares[file][rank];
    squares[file][rank] = nullptr;
}

void Board::makeMove(const Move& move) {
    Square from = move.getFrom();
    Square to = move.getTo();

    Piece* piece = getPieceAt(from.getFile(), from.getRank());
    if (!piece) return;

    if (piece->getType() == PieceType::KING) {
        if (piece->getColor() == Color::WHITE) whiteKingMoved = true;
        else blackKingMoved = true;
    }

    if (piece->getType() == PieceType::ROOK) {
        if (piece->getColor() == Color::WHITE) {
            if (from.getFile() == 0 && from.getRank() == 0) whiteRookA_Moved = true;
            if (from.getFile() == 7 && from.getRank() == 0) whiteRookH_Moved = true;
        } else {
            if (from.getFile() == 0 && from.getRank() == 7) blackRookA_Moved = true;
            if (from.getFile() == 7 && from.getRank() == 7) blackRookH_Moved = true;
        }
    }

    removePieceAt(to.getFile(), to.getRank());
    squares[from.getFile()][from.getRank()] = nullptr;

    piece->setPosition(to.getFile(), to.getRank());
    squares[to.getFile()][to.getRank()] = piece;
}

void Board::undoMove(const Move& move, Piece* captured, Square from, Square to) {
    Piece* piece = getPieceAt(to.getFile(), to.getRank());
    if (!piece) return;

    squares[to.getFile()][to.getRank()] = nullptr;
    piece->setPosition(from.getFile(), from.getRank());
    squares[from.getFile()][from.getRank()] = piece;

    if (captured) {
        squares[to.getFile()][to.getRank()] = captured;
    }
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

std::vector<Piece*>& Board::getPieces(Color color) {
    return (color == Color::WHITE) ? whitePieces : blackPieces;
}

const std::vector<Piece*>& Board::getPieces(Color color) const {
    return (color == Color::WHITE) ? whitePieces : blackPieces;
}

bool Board::isLegalMove(const Move& move, Color turn) const {
    Board tmp = *this;

    Piece* captured = tmp.getPieceAt(move.getTo().getFile(), move.getTo().getRank());
    tmp.makeMove(move);

    Piece* king = nullptr;
    for (Piece* p : tmp.getPieces(turn)) {
        if (p->getType() == PieceType::KING) {
            king = p;
            break;
        }
    }
    if (!king) return false;

    return !tmp.isSquareAttacked(king->getFile(), king->getRank(),
                                 (turn == Color::WHITE) ? Color::BLACK : Color::WHITE);
}

bool Board::canCastleKingSide(Color turn) const {
    if (turn == Color::WHITE) {
        if (whiteKingMoved || whiteRookH_Moved) return false;
        if (!isEmpty(5,0) || !isEmpty(6,0)) return false;
        if (isSquareAttacked(4,0, Color::BLACK) ||
            isSquareAttacked(5,0, Color::BLACK) ||
            isSquareAttacked(6,0, Color::BLACK))
            return false;
        return true;
    } else {
        if (blackKingMoved || blackRookH_Moved) return false;
        if (!isEmpty(5,7) || !isEmpty(6,7)) return false;
        if (isSquareAttacked(4,7, Color::WHITE) ||
            isSquareAttacked(5,7, Color::WHITE) ||
            isSquareAttacked(6,7, Color::WHITE))
            return false;
        return true;
    }
}

bool Board::canCastleQueenSide(Color turn) const {
    if (turn == Color::WHITE) {
        if (whiteKingMoved || whiteRookA_Moved) return false;
        if (!isEmpty(1,0) || !isEmpty(2,0) || !isEmpty(3,0)) return false;
        if (isSquareAttacked(4,0, Color::BLACK) ||
            isSquareAttacked(3,0, Color::BLACK) ||
            isSquareAttacked(2,0, Color::BLACK))
            return false;
        return true;
    } else {
        if (blackKingMoved || blackRookA_Moved) return false;
        if (!isEmpty(1,7) || !isEmpty(2,7) || !isEmpty(3,7)) return false;
        if (isSquareAttacked(4,7, Color::WHITE) ||
            isSquareAttacked(3,7, Color::WHITE) ||
            isSquareAttacked(2,7, Color::WHITE))
            return false;
        return true;
    }
}

Move Board::getCastlingMove(Color turn, bool kingSide) const {
    if (turn == Color::WHITE) {
        if (kingSide) return Move(Square(4,0), Square(6,0));
        return Move(Square(4,0), Square(2,0));
    } else {
        if (kingSide) return Move(Square(4,7), Square(6,7));
        return Move(Square(4,7), Square(2,7));
    }
}

void Board::performCastling(Color turn, bool kingSide) {
    if (!canCastleKingSide(turn) && kingSide) return;
    if (!canCastleQueenSide(turn) && !kingSide) return;

    if (turn == Color::WHITE) {
        if (kingSide) {
            makeMove(Move(Square(4,0), Square(6,0)));
            makeMove(Move(Square(7,0), Square(5,0)));
        } else {
            makeMove(Move(Square(4,0), Square(2,0)));
            makeMove(Move(Square(0,0), Square(3,0)));
        }
    } else {
        if (kingSide) {
            makeMove(Move(Square(4,7), Square(6,7)));
            makeMove(Move(Square(7,7), Square(5,7)));
        } else {
            makeMove(Move(Square(4,7), Square(2,7)));
            makeMove(Move(Square(0,7), Square(3,7)));
        }
    }
}
