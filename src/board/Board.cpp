#include "board/Board.h"
#include "board/Square.h"
#include "board/Move.h"
#include "pieces/King.h"
#include "pieces/Rook.h"
#include "pieces/Queen.h"
#include "pieces/Bishop.h"
#include "pieces/Knight.h"

#include <algorithm>
#include <cmath>

Board::Board() {
    for (int f = 0; f < 8; f++) {
        for (int r = 0; r < 8; r++) {
            squares[f][r] = nullptr;
        }
    }
}

Board::Board(const Board& other) {
    whiteKingMoved = other.whiteKingMoved;
    blackKingMoved = other.blackKingMoved;
    whiteRookA_Moved = other.whiteRookA_Moved;
    whiteRookH_Moved = other.whiteRookH_Moved;
    blackRookA_Moved = other.blackRookA_Moved;
    blackRookH_Moved = other.blackRookH_Moved;

    for (int f = 0; f < 8; f++) {
        for (int r = 0; r < 8; r++) {
            if (other.squares[f][r]) {
                squares[f][r] = other.squares[f][r]->clone();
                if (squares[f][r]->getColor() == Color::WHITE)
                    whitePieces.push_back(squares[f][r]);
                else
                    blackPieces.push_back(squares[f][r]);
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
    if (piece->getColor() == Color::WHITE)
        whitePieces.push_back(piece);
    else
        blackPieces.push_back(piece);
}

void Board::removePieceAt(int file, int rank) {
    if (!isInside(file, rank)) return;

    Piece* piece = squares[file][rank];
    if (!piece) return;

    auto& vec = (piece->getColor() == Color::WHITE)
        ? whitePieces : blackPieces;

    vec.erase(std::remove(vec.begin(), vec.end(), piece), vec.end());

    delete piece;
    squares[file][rank] = nullptr;
}

void Board::makeMove(const Move& move) {
    Square from = move.getFrom();
    Square to   = move.getTo();

    Piece* piece = getPieceAt(from.getFile(), from.getRank());
    if (!piece) return;

    if (piece->getType() == PieceType::KING &&
        std::abs(to.getFile() - from.getFile()) == 2) {

        bool kingSide = to.getFile() > from.getFile();
        squares[from.getFile()][from.getRank()] = nullptr;
        piece->setPosition(to.getFile(), to.getRank());
        squares[to.getFile()][to.getRank()] = piece;

        if (piece->getColor() == Color::WHITE) {
            whiteKingMoved = true;
        } else {
            blackKingMoved = true;
        }
        return;
    }

    removePieceAt(to.getFile(), to.getRank());
    squares[from.getFile()][from.getRank()] = nullptr;

    if (piece->getType() == PieceType::PAWN && move.isPromotion()) {

        Color color = piece->getColor();
        auto& vec = (color == Color::WHITE) ? whitePieces : blackPieces;
        vec.erase(std::remove(vec.begin(), vec.end(), piece), vec.end());
        delete piece;

        Piece* newPiece = nullptr;
        switch (move.getPromotion().value()) {
            case PieceType::QUEEN:
                newPiece = new Queen(color, to.getFile(), to.getRank());
                break;
            case PieceType::ROOK:
                newPiece = new Rook(color, to.getFile(), to.getRank());
                break;
            case PieceType::BISHOP:
                newPiece = new Bishop(color, to.getFile(), to.getRank());
                break;
            case PieceType::KNIGHT:
                newPiece = new Knight(color, to.getFile(), to.getRank());
                break;
            default:
                newPiece = new Queen(color, to.getFile(), to.getRank());
                break;
        }

        squares[to.getFile()][to.getRank()] = newPiece;
        vec.push_back(newPiece);
    }
    else {
        piece->setPosition(to.getFile(), to.getRank());
        squares[to.getFile()][to.getRank()] = piece;
    }
}
