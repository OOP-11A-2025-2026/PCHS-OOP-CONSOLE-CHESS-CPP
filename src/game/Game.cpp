#include "game/Game.h"
#include "pieces/Piece.h"
#include "pieces/Pawn.h"
#include "pieces/Rook.h"
#include "pieces/Knight.h"
#include "pieces/Bishop.h"
#include "pieces/Queen.h"
#include "pieces/King.h"
#include <sstream>

/**
 * Constructs a new game with the board in starting position.
 */
Game::Game() 
    : board(), 
      currentPlayer(Color::WHITE), 
      state(GameState::ONGOING),
      drawOffered(false),
      drawOfferedBy(std::nullopt),
      moveHistory() {
    initializeBoard();
}

/**
 * Initializes the chess board with pieces in starting positions.
 */
void Game::initializeBoard() {
    // Set up pawns
    for (int file = 0; file < 8; file++) {
        board.placePiece(new Pawn(Color::WHITE, file, 1));
        board.placePiece(new Pawn(Color::BLACK, file, 6));
    }

    // Set up white pieces
    board.placePiece(new Rook(Color::WHITE, 0, 0));
    board.placePiece(new Rook(Color::WHITE, 7, 0));
    board.placePiece(new Knight(Color::WHITE, 1, 0));
    board.placePiece(new Knight(Color::WHITE, 6, 0));
    board.placePiece(new Bishop(Color::WHITE, 2, 0));
    board.placePiece(new Bishop(Color::WHITE, 5, 0));
    board.placePiece(new Queen(Color::WHITE, 3, 0));
    board.placePiece(new King(Color::WHITE, 4, 0));

    // Set up black pieces
    board.placePiece(new Rook(Color::BLACK, 0, 7));
    board.placePiece(new Rook(Color::BLACK, 7, 7));
    board.placePiece(new Knight(Color::BLACK, 1, 7));
    board.placePiece(new Knight(Color::BLACK, 6, 7));
    board.placePiece(new Bishop(Color::BLACK, 2, 7));
    board.placePiece(new Bishop(Color::BLACK, 5, 7));
    board.placePiece(new Queen(Color::BLACK, 3, 7));
    board.placePiece(new King(Color::BLACK, 4, 7));
}

/**
 * Gets the current game board.
 */
Board& Game::getBoard() {
    return board;
}

/**
 * Gets the current game board (const version).
 */
const Board& Game::getBoard() const {
    return board;
}

/**
 * Gets the current player's color.
 */
Color Game::getCurrentPlayer() const {
    return currentPlayer;
}

/**
 * Sets the current player.
 */
void Game::setCurrentPlayer(Color color) {
    currentPlayer = color;
}

/**
 * Gets the current game state.
 */
GameState Game::getState() const {
    return state;
}

/**
 * Checks if a draw has been offered.
 */
bool Game::isDrawOffered() const {
    return drawOffered;
}

/**
 * Gets the color that offered the draw.
 */
std::optional<Color> Game::getDrawOfferedBy() const {
    return drawOfferedBy;
}

/**
 * Current player offers a draw.
 */
void Game::offerDraw() {
    drawOffered = true;
    drawOfferedBy = currentPlayer;
}

/**
 * Current player accepts the offered draw.
 */
void Game::acceptDraw() {
    if (drawOffered && drawOfferedBy.has_value() && drawOfferedBy.value() != currentPlayer) {
        state = GameState::DRAW;
    }
}

/**
 * Current player declines the offered draw.
 */
void Game::declineDraw() {
    drawOffered = false;
    drawOfferedBy = std::nullopt;
}

/**
 * Current player resigns the game.
 */
void Game::resign() {
    state = GameState::RESIGNED;
}

/**
 * Attempts to make a move on the board.
 */
bool Game::makeMove(const Move& move) {
    // Check if game is already over
    if (state == GameState::CHECKMATE ||
        state == GameState::STALEMATE ||
        state == GameState::DRAW ||
        state == GameState::RESIGNED) {
        return false;
    }

    Square from = move.getFrom();
    Square to = move.getTo();

    Piece* piece = board.getPieceAt(from);

    // Check if there's a piece and it belongs to current player
    if (piece == nullptr || piece->getColor() != currentPlayer) {
        return false;
    }

    // Check if the move is in the piece's legal moves
    bool legal = false;
    std::vector<Move> legalMoves = piece->getLegalMoves(board);
    for (const Move& m : legalMoves) {
        if (m.getTo().getFile() == to.getFile() &&
            m.getTo().getRank() == to.getRank()) {
            legal = true;
            break;
        }
    }
    if (!legal) {
        return false;
    }

    // Check if move would leave king in check
    Board testBoard = board.clone();
    testBoard.applyMove(move);
    std::optional<Square> kingSquare = findKing(testBoard, currentPlayer);
    if (kingSquare.has_value() &&
        testBoard.isSquareAttacked(kingSquare.value(), getOpponentColor(currentPlayer))) {
        return false;
    }

    // Record move in SAN notation before applying
    std::string san = moveToSAN(move, piece);

    // Apply the move
    board.applyMove(move);
    moveHistory.push_back(san);

    // Auto-decline draw offer if one was made by opponent
    if (drawOffered && drawOfferedBy.has_value() && drawOfferedBy.value() != currentPlayer) {
        declineDraw();
    }

    // Switch players
    currentPlayer = getOpponentColor(currentPlayer);

    // Update game state
    updateGameState();

    if (state == GameState::CHECK) {
        moveHistory.back() += "+";
    } else if (state == GameState::CHECKMATE) {
        moveHistory.back() += "#";
    }

    return true;
}

/**
 * Updates the game state (check, checkmate, stalemate, ongoing).
 */
void Game::updateGameState() {
    std::optional<Square> kingSquare = findKing(board, currentPlayer);
    if (!kingSquare.has_value()) {
        return;
    }

    bool inCheck = board.isSquareAttacked(kingSquare.value(), getOpponentColor(currentPlayer));
    bool hasLegalMoves = hasAnyLegalMoves(currentPlayer);

    if (inCheck) {
        if (!hasLegalMoves) {
            state = GameState::CHECKMATE;
        } else {
            state = GameState::CHECK;
        }
    } else {
        if (!hasLegalMoves) {
            state = GameState::STALEMATE;
        } else {
            state = GameState::ONGOING;
        }
    }
}

/**
 * Checks if the specified color has any legal moves available.
 */
bool Game::hasAnyLegalMoves(Color color) {
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            Square square(file, rank);
            Piece* piece = board.getPieceAt(square);
            if (piece != nullptr && piece->getColor() == color) {
                std::vector<Move> moves = piece->getLegalMoves(board);
                for (const Move& move : moves) {
                    Board testBoard = board.clone();
                    testBoard.applyMove(move);
                    std::optional<Square> kingSquare = findKing(testBoard, color);
                    if (kingSquare.has_value() && 
                        !testBoard.isSquareAttacked(kingSquare.value(), getOpponentColor(color))) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

/**
 * Finds the king's square for the specified color.
 */
std::optional<Square> Game::findKing(const Board& board, Color color) const {
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            Square square(file, rank);
            Piece* piece = board.getPieceAt(square);
            if (piece != nullptr && 
                piece->getType() == PieceType::KING && 
                piece->getColor() == color) {
                return square;
            }
        }
    }
    return std::nullopt;
}

/**
 * Gets the opponent's color.
 */
Color Game::getOpponentColor(Color color) const {
    return color == Color::WHITE ? Color::BLACK : Color::WHITE;
}

/**
 * Gets the winner of the game.
 */
std::string Game::getWinner() const {
    if (state == GameState::CHECKMATE || state == GameState::RESIGNED) {
        return currentPlayer == Color::WHITE ? "Black" : "White";
    }
    return "";
}

/**
 * Gets the move history in SAN notation.
 */
std::vector<std::string> Game::getMoveHistory() const {
    return moveHistory;
}

/**
 * Sets the move history.
 */
void Game::setMoveHistory(const std::vector<std::string>& history) {
    moveHistory = history;
}

/**
 * Converts a move to Standard Algebraic Notation (SAN).
 */
std::string Game::moveToSAN(const Move& move, const Piece* piece) {
    Square from = move.getFrom();
    Square to = move.getTo();
    std::ostringstream sb;

    // Castling notation
    if (piece->getType() == PieceType::KING && 
        std::abs(to.getFile() - from.getFile()) == 2) {
        return to.getFile() > from.getFile() ? "O-O" : "O-O-O";
    }

    // Piece symbol (except for pawns)
    if (piece->getType() != PieceType::PAWN) {
        switch (piece->getType()) {
            case PieceType::KING:   sb << 'K'; break;
            case PieceType::QUEEN:  sb << 'Q'; break;
            case PieceType::ROOK:   sb << 'R'; break;
            case PieceType::BISHOP: sb << 'B'; break;
            case PieceType::KNIGHT: sb << 'N'; break;
            default: sb << '?'; break;
        }
    }

    // Check if it's a capture
    Piece* target = board.getPieceAt(to);
    bool isCapture = target != nullptr;
    
    // En passant capture
    if (piece->getType() == PieceType::PAWN && 
        from.getFile() != to.getFile() && 
        target == nullptr) {
        isCapture = true;
    }

    // For pawn captures, include the file
    if (piece->getType() == PieceType::PAWN && isCapture) {
        sb << static_cast<char>('a' + from.getFile());
    }

    // Capture symbol
    if (isCapture) {
        sb << 'x';
    }

    // Destination square
    sb << static_cast<char>('a' + to.getFile());
    sb << static_cast<char>('1' + to.getRank());

    // Promotion
    if (move.getPromotion().has_value()) {
        sb << '=';
        switch (move.getPromotion().value()) {
            case PieceType::QUEEN:  sb << 'Q'; break;
            case PieceType::ROOK:   sb << 'R'; break;
            case PieceType::BISHOP: sb << 'B'; break;
            case PieceType::KNIGHT: sb << 'N'; break;
            default: sb << 'Q'; break;
        }
    }

    return sb.str();
}