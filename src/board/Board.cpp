#include "board/Board.h"
#include "board/Move.h"
#include "board/Square.h"

#include "pieces/Piece.h"
#include "pieces/King.h"
#include "pieces/Queen.h"
#include "pieces/Rook.h"
#include "pieces/Bishop.h"
#include "pieces/Knight.h"
#include "pieces/Pawn.h"

#include <algorithm>
#include <cmath>

Board::Board()
    : enPassantTarget(0, 0), enPassantAvailable(false),
      whiteKingMoved(false), blackKingMoved(false),
      whiteRookA_Moved(false), whiteRookH_Moved(false),
      blackRookA_Moved(false), blackRookH_Moved(false),
      lastMove(nullptr) {
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
    
    // Copy lastMove if it exists
    if (other.lastMove) {
        lastMove = new Move(
            Square(other.lastMove->getFrom().getFile(), other.lastMove->getFrom().getRank()),
            Square(other.lastMove->getTo().getFile(), other.lastMove->getTo().getRank())
        );
    } else {
        lastMove = nullptr;
    }
}

Board::~Board() {
    for (int f = 0; f < 8; f++)
        for (int r = 0; r < 8; r++)
            delete squares[f][r];
    delete lastMove;
}

bool Board::isInside(int file, int rank) const {
    return file >= 0 && file < 8 && rank >= 0 && rank < 8;
}

Piece* Board::getPieceAt(int file, int rank) const {
    if (!isInside(file, rank)) return nullptr;
    return squares[file][rank];
}

Piece* Board::getPieceAt(const Square& square) const {
    return squares[square.getFile()][square.getRank()];
}

void Board::setPieceAt(const Square& square, Piece* piece) {
    squares[square.getFile()][square.getRank()] = piece;
}

bool Board::isInBounds(const Square& square) const {
    int f = square.getFile();
    int r = square.getRank();
    return f >= 0 && f < 8 && r >= 0 && r < 8;
}

bool Board::isOwnPiece(const Square& square, Color color) const {
    if (!isInBounds(square)) return false;
    Piece* p = getPieceAt(square);
    return p != nullptr && p->getColor() == color;
}

bool Board::isPathClear(const Square& from, const Square& to) const {
    if (!isInBounds(from) || !isInBounds(to)) return false;

    int df = to.getFile() - from.getFile();
    int dr = to.getRank() - from.getRank();

    int stepF = (df > 0) ? 1 : (df < 0) ? -1 : 0;
    int stepR = (dr > 0) ? 1 : (dr < 0) ? -1 : 0;

    // Check if it's a valid straight or diagonal line
    if (stepF != 0 && stepR != 0 && std::abs(df) != std::abs(dr)) {
        return false;
    }

    int curF = from.getFile() + stepF;
    int curR = from.getRank() + stepR;

    while (curF != to.getFile() || curR != to.getRank()) {
        Piece* p = squares[curR][curF];
        if (p != nullptr) return false;
        curF += stepF;
        curR += stepR;
    }
    return true;
}

Board Board::clone() const {
    return Board(*this);
}

Move* Board::getLastMove() const {
    return lastMove;
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

bool Board::sameSquare(const Square& a, const Square& b) const {
    return a.getFile() == b.getFile() && a.getRank() == b.getRank();
}

bool Board::isPawnPromotion(const Piece* piece, const Square& to) const {
    if (piece == nullptr) return false;
    if (piece->getType() != PieceType::PAWN) return false;

    if (piece->getColor() == Color::WHITE && to.getRank() == 7) return true;
    if (piece->getColor() == Color::BLACK && to.getRank() == 0) return true;

    return false;
}

bool Board::isEnPassantMove(const Move& move, const Piece* moving) const {
    if (moving == nullptr) return false;
    if (moving->getType() != PieceType::PAWN) return false;
    if (lastMove == nullptr) return false;

    Square from = move.getFrom();
    Square to = move.getTo();

    int df = to.getFile() - from.getFile();
    int dr = to.getRank() - from.getRank();

    if (std::abs(df) != 1) return false;
    if (std::abs(dr) != 1) return false;

    if (getPieceAt(to) != nullptr) return false;

    Square lmFrom = lastMove->getFrom();
    Square lmTo = lastMove->getTo();
    Piece* lastMovedPiece = getPieceAt(lmTo);
    if (lastMovedPiece == nullptr || lastMovedPiece->getType() != PieceType::PAWN) return false;
    if (std::abs(lmTo.getRank() - lmFrom.getRank()) != 2) return false;

    int passedRank = (lmFrom.getRank() + lmTo.getRank()) / 2;
    Square passedOver(lmTo.getFile(), passedRank);

    if (sameSquare(to, passedOver)) {
        if (lmTo.getRank() == from.getRank() && std::abs(lmTo.getFile() - from.getFile()) == 1) {
            return true;
        }
    }
    return false;
}

Piece* Board::applyMove(const Move& move) {
    Square from = move.getFrom();
    Square to = move.getTo();
    if (!isInBounds(from) || !isInBounds(to)) return nullptr;

    Piece* moving = getPieceAt(from);
    if (moving == nullptr) return nullptr;

    Piece* captured = nullptr;

    // Handle en passant capture
    if (isEnPassantMove(move, moving)) {
        Square lastLanding = lastMove->getTo();
        captured = getPieceAt(lastLanding);
        setPieceAt(lastLanding, nullptr);
    } else {
        captured = getPieceAt(to);
    }

    // Move the piece
    setPieceAt(to, moving);
    setPieceAt(from, nullptr);
    moving->setPosition(to.getFile(), to.getRank());

    // Handle castling - move the rook
    if (moving->getType() == PieceType::KING) {
        int dx = to.getFile() - from.getFile();
        if (dx == 2) {  // Kingside castle
            int rank = from.getRank();
            Piece* rook = getPieceAt(Square(7, rank));
            if (rook != nullptr && rook->getType() == PieceType::ROOK) {
                setPieceAt(Square(5, rank), rook);
                setPieceAt(Square(7, rank), nullptr);
                rook->setPosition(5, rank);
            }
        } else if (dx == -2) {  // Queenside castle
            int rank = from.getRank();
            Piece* rook = getPieceAt(Square(0, rank));
            if (rook != nullptr && rook->getType() == PieceType::ROOK) {
                setPieceAt(Square(3, rank), rook);
                setPieceAt(Square(0, rank), nullptr);
                rook->setPosition(3, rank);
            }
        }
    }

    // Handle pawn promotion
    PieceType promoType = move.getPromotion().value_or(PieceType::PAWN);
    if (move.getPromotion().has_value() && moving->getType() == PieceType::PAWN) {
        Piece* promoted = nullptr;
        int file = to.getFile();
        int rank = to.getRank();
        switch (promoType) {
            case PieceType::QUEEN:  promoted = new Queen(moving->getColor(), file, rank); break;
            case PieceType::ROOK:   promoted = new Rook(moving->getColor(), file, rank); break;
            case PieceType::BISHOP: promoted = new Bishop(moving->getColor(), file, rank); break;
            case PieceType::KNIGHT: promoted = new Knight(moving->getColor(), file, rank); break;
            default: promoted = new Queen(moving->getColor(), file, rank); break;
        }
        setPieceAt(to, promoted);
    } else if (isPawnPromotion(moving, to)) {
        // Auto-promote to Queen if no promotion specified
        Piece* promoted = new Queen(moving->getColor(), to.getFile(), to.getRank());
        setPieceAt(to, promoted);
    }

    // Update lastMove
    delete lastMove;
    lastMove = new Move(from, to);

    return captured;
}

bool Board::isSquareAttacked(const Square& target, Color byColor) const {
    int tx = target.getFile();
    int ty = target.getRank();

    // Pawn attacks
    int pawnDir = (byColor == Color::WHITE) ? 1 : -1;
    int pFiles[] = {tx - 1, tx + 1};
    for (int pf : pFiles) {
        int pr = ty - pawnDir;
        if (pf >= 0 && pf < 8 && pr >= 0 && pr < 8) {
            Piece* p = squares[pr][pf];
            if (p != nullptr && p->getColor() == byColor && p->getType() == PieceType::PAWN) 
                return true;
        }
    }

    // Knight attacks
    int knightOffsets[][2] = {{1,2},{2,1},{2,-1},{1,-2},{-1,-2},{-2,-1},{-2,1},{-1,2}};
    for (auto& o : knightOffsets) {
        int fx = tx + o[0];
        int ry = ty + o[1];
        if (fx >= 0 && fx < 8 && ry >= 0 && ry < 8) {
            Piece* p = squares[ry][fx];
            if (p != nullptr && p->getColor() == byColor && p->getType() == PieceType::KNIGHT) 
                return true;
        }
    }

    // King attacks
    for (int df = -1; df <= 1; df++) {
        for (int dr = -1; dr <= 1; dr++) {
            if (df == 0 && dr == 0) continue;
            int fx = tx + df;
            int ry = ty + dr;
            if (fx >= 0 && fx < 8 && ry >= 0 && ry < 8) {
                Piece* p = squares[ry][fx];
                if (p != nullptr && p->getColor() == byColor && p->getType() == PieceType::KING) 
                    return true;
            }
        }
    }

    // Sliding pieces (Rook, Bishop, Queen)
    int directions[][2] = {{1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}};
    for (int i = 0; i < 8; i++) {
        int df = directions[i][0];
        int dr = directions[i][1];
        int fx = tx + df;
        int ry = ty + dr;
        while (fx >= 0 && fx < 8 && ry >= 0 && ry < 8) {
            Piece* p = squares[ry][fx];
            if (p != nullptr) {
                if (p->getColor() == byColor) {
                    PieceType t = p->getType();
                    bool orth = (i < 4);  // orthogonal directions
                    bool diag = (i >= 4); // diagonal directions
                    if ((orth && (t == PieceType::ROOK || t == PieceType::QUEEN)) ||
                        (diag && (t == PieceType::BISHOP || t == PieceType::QUEEN))) {
                        return true;
                    }
                }
                break;
            }
            fx += df; 
            ry += dr;
        }
    }

    return false;
}

bool Board::simulateMoveAndDetectSelfCheck(const Move& move, Color movingColor) const {
    Board copy = this->clone();

    Square from = move.getFrom();
    Square to = move.getTo();
    Move copyMove(Square(from.getFile(), from.getRank()), Square(to.getFile(), to.getRank()));

    copy.applyMove(copyMove);

    Square kingSquare(0, 0);
    bool kingFound = false;
    for (int r = 0; r < 8 && !kingFound; r++) {
        for (int f = 0; f < 8; f++) {
            Piece* p = copy.getPieceAt(Square(f, r));
            if (p != nullptr && p->getColor() == movingColor && p->getType() == PieceType::KING) {
                kingSquare = Square(f, r);
                kingFound = true;
                break;
            }
        }
    }

    if (!kingFound) {
        return false;
    }

    Color opponent = (movingColor == Color::WHITE) ? Color::BLACK : Color::WHITE;
    return copy.isSquareAttacked(kingSquare, opponent);
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
    // Leave empty for now
}

bool Board::isSquareAttacked(int file, int rank, Color byColor) const {
    return isSquareAttacked(Square(file, rank), byColor);
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