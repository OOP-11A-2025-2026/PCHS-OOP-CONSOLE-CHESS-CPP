#include "board/Board.h"
#include "board/Move.h"
#include "board/Square.h"

#include "pieces/Piece.h"
#include "pieces/King.h"
#include "pieces/Queen.h"
#include "pieces/Rook.h"
#include "pieces/Bishop.h"
#include "pieces/Knight.h"

#include <algorithm>
#include <cmath>

Board::Board()
    : enPassantTarget(0, 0), enPassantAvailable(false),
      whiteKingMoved(false), blackKingMoved(false),
      whiteRookA_Moved(false), whiteRookH_Moved(false),
      blackRookA_Moved(false), blackRookH_Moved(false) {
    for (int f = 0; f < 8; f++)
        for (int r = 0; r < 8; r++)
            squares[f][r] = nullptr;
}

Board::Board(const Board& other)
    : enPassantTarget(other.enPassantTarget),
      enPassantAvailable(other.enPassantAvailable),
      whiteKingMoved(other.whiteKingMoved),
      blackKingMoved(other.blackKingMoved),
      whiteRookA_Moved(other.whiteRookA_Moved),
      whiteRookH_Moved(other.whiteRookH_Moved),
      blackRookA_Moved(other.blackRookA_Moved),
      blackRookH_Moved(other.blackRookH_Moved) {
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
    for (int f = 0; f < 8; f++)
        for (int r = 0; r < 8; r++)
            delete squares[f][r];
}

bool Board::isInside(int file, int rank) const {
    return file >= 0 && file < 8 && rank >= 0 && rank < 8;
}

Piece* Board::getPieceAt(int file, int rank) const {
    if (!isInside(file, rank)) return nullptr;
    return squares[file][rank];
}

std::vector<Piece*>& Board::getPieces(Color color) {
    return (color == Color::WHITE) ? whitePieces : blackPieces;
}

bool Board::isEnPassantAvailable() const {
    return enPassantAvailable;
}

Square Board::getEnPassantTarget() const {
    return enPassantTarget;
}


const std::vector<Piece*>& Board::getPieces(Color color) const {
    return (color == Color::WHITE) ? whitePieces : blackPieces;
}

bool Board::isEmpty(int file, int rank) const {
    return getPieceAt(file, rank) == nullptr;
}

bool Board::isEnemy(int file, int rank, Color myColor) const {
    Piece* p = getPieceAt(file, rank);
    return p && p->getColor() != myColor;
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
    Piece* p = squares[file][rank];
    if (!p) return;
    auto& vec = (p->getColor() == Color::WHITE) ? whitePieces : blackPieces;
    vec.erase(std::remove(vec.begin(), vec.end(), p), vec.end());
    delete p;
    squares[file][rank] = nullptr;
}

void Board::makeMove(const Move& move) {
    Square from = move.getFrom();
    Square to = move.getTo();
    Piece* piece = getPieceAt(from.getFile(), from.getRank());
    if (!piece) return;

    enPassantAvailable = false;

    if (piece->getType() == PieceType::KING &&
        std::abs(to.getFile() - from.getFile()) == 2) {

        bool kingSide = to.getFile() > from.getFile();

        squares[from.getFile()][from.getRank()] = nullptr;
        piece->setPosition(to.getFile(), to.getRank());
        squares[to.getFile()][to.getRank()] = piece;

        if (piece->getColor() == Color::WHITE) {
            whiteKingMoved = true;
            if (kingSide) {
                Piece* rook = getPieceAt(7, 0);
                squares[7][0] = nullptr;
                rook->setPosition(5, 0);
                squares[5][0] = rook;
                whiteRookH_Moved = true;
            } else {
                Piece* rook = getPieceAt(0, 0);
                squares[0][0] = nullptr;
                rook->setPosition(3, 0);
                squares[3][0] = rook;
                whiteRookA_Moved = true;
            }
        } else {
            blackKingMoved = true;
            if (kingSide) {
                Piece* rook = getPieceAt(7, 7);
                squares[7][7] = nullptr;
                rook->setPosition(5, 7);
                squares[5][7] = rook;
                blackRookH_Moved = true;
            } else {
                Piece* rook = getPieceAt(0, 7);
                squares[0][7] = nullptr;
                rook->setPosition(3, 7);
                squares[3][7] = rook;
                blackRookA_Moved = true;
            }
        }
        return;
    }

    if (piece->getType() == PieceType::PAWN &&
        std::abs(to.getRank() - from.getRank()) == 2) {

        int epRank = (from.getRank() + to.getRank()) / 2;
        enPassantTarget = Square(to.getFile(), epRank);
        enPassantAvailable = true;
    }

    if (piece->getType() == PieceType::PAWN &&
        from.getFile() != to.getFile() &&
        isEmpty(to.getFile(), to.getRank()) &&
        enPassantAvailable &&
        to == enPassantTarget) {

        int capturedRank = from.getRank();
        int capturedFile = to.getFile();
        removePieceAt(capturedFile, capturedRank);
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
            case PieceType::QUEEN:  newPiece = new Queen(color, to.getFile(), to.getRank()); break;
            case PieceType::ROOK:   newPiece = new Rook(color, to.getFile(), to.getRank()); break;
            case PieceType::BISHOP: newPiece = new Bishop(color, to.getFile(), to.getRank()); break;
            case PieceType::KNIGHT: newPiece = new Knight(color, to.getFile(), to.getRank()); break;
            default: newPiece = new Queen(color, to.getFile(), to.getRank()); break;
        }

        squares[to.getFile()][to.getRank()] = newPiece;
        vec.push_back(newPiece);
        return;
    }

    piece->setPosition(to.getFile(), to.getRank());
    squares[to.getFile()][to.getRank()] = piece;
}

void Board::undoMove(const Move& move, Piece* captured, Square from, Square to) {
    // Тук не е нужно за момента, оставям празно
}

bool Board::isSquareAttacked(int file, int rank, Color byColor) const {
    const auto& vec = (byColor == Color::WHITE) ? whitePieces : blackPieces;
    for (Piece* p : vec) {
        for (const Move& m : p->getLegalMoves(*this)) {
            if (m.getTo().getFile() == file && m.getTo().getRank() == rank)
                return true;
        }
    }
    return false;
}

bool Board::isLegalMove(const Move& move, Color turn) const {
    Board tmp(*this);
    tmp.makeMove(move);
    const auto& vec = tmp.getPieces(turn);
    Piece* king = nullptr;
    for (Piece* p : vec)
        if (p->getType() == PieceType::KING)
            king = p;
    if (!king) return false;
    return !tmp.isSquareAttacked(
        king->getFile(),
        king->getRank(),
        turn == Color::WHITE ? Color::BLACK : Color::WHITE
    );
}

bool Board::canCastleKingSide(Color turn) const {
    if (turn == Color::WHITE) {
        if (whiteKingMoved || whiteRookH_Moved) return false;
        if (!isEmpty(5,0) || !isEmpty(6,0)) return false;
        return !isSquareAttacked(4,0,Color::BLACK)
            && !isSquareAttacked(5,0,Color::BLACK)
            && !isSquareAttacked(6,0,Color::BLACK);
    } else {
        if (blackKingMoved || blackRookH_Moved) return false;
        if (!isEmpty(5,7) || !isEmpty(6,7)) return false;
        return !isSquareAttacked(4,7,Color::WHITE)
            && !isSquareAttacked(5,7,Color::WHITE)
            && !isSquareAttacked(6,7,Color::WHITE);
    }
}

bool Board::canCastleQueenSide(Color turn) const {
    if (turn == Color::WHITE) {
        if (whiteKingMoved || whiteRookA_Moved) return false;
        if (!isEmpty(1,0) || !isEmpty(2,0) || !isEmpty(3,0)) return false;
        return !isSquareAttacked(4,0,Color::BLACK)
            && !isSquareAttacked(3,0,Color::BLACK)
            && !isSquareAttacked(2,0,Color::BLACK);
    } else {
        if (blackKingMoved || blackRookA_Moved) return false;
        if (!isEmpty(1,7) || !isEmpty(2,7) || !isEmpty(3,7)) return false;
        return !isSquareAttacked(4,7,Color::WHITE)
            && !isSquareAttacked(3,7,Color::WHITE)
            && !isSquareAttacked(2,7,Color::WHITE);
    }
}

Move Board::getCastlingMove(Color turn, bool kingSide) const {
    if (turn == Color::WHITE)
        return kingSide ? Move(Square(4,0), Square(6,0))
                        : Move(Square(4,0), Square(2,0));
    return kingSide ? Move(Square(4,7), Square(6,7))
                    : Move(Square(4,7), Square(2,7));
}

void Board::performCastling(Color turn, bool kingSide) {
    makeMove(getCastlingMove(turn, kingSide));
}
