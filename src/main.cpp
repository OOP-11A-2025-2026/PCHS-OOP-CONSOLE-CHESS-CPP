#include <iostream>
#include "board/Board.h"
#include "board/Square.h"
#include "board/Move.h"
#include "pieces/Piece.h"
#include "pieces/Pawn.h"
#include "pieces/Rook.h"
#include "pieces/Knight.h"
#include "pieces/Bishop.h"
#include "pieces/Queen.h"
#include "pieces/King.h"
#include "enums/Color.h"
#include "enums/PieceType.h"
#include "cli/BoardPrinter.h"
#include "cli/PieceRenderer.h"

void printTestHeader(const std::string& testName) {
    std::cout << "\n========================================" << std::endl;
    std::cout << "  " << testName << std::endl;
    std::cout << "========================================" << std::endl;
}

void testEmptyBoard() {
    printTestHeader("TEST 1: Empty Board");
    Board board;
    std::cout << "Expected: Empty 8x8 board with alternating colors" << std::endl;
    BoardPrinter::print(board);
}

void testInitialPosition() {
    printTestHeader("TEST 2: Standard Starting Position");
    Board board;
    
    // Set up pawns
    for (int file = 0; file < 8; file++) {
        board.setPieceAt(Square(file, 1), new Pawn(Color::WHITE, file, 1));
        board.setPieceAt(Square(file, 6), new Pawn(Color::BLACK, file, 6));
    }

    // Set up white pieces
    board.setPieceAt(Square(0, 0), new Rook(Color::WHITE, 0, 0));
    board.setPieceAt(Square(7, 0), new Rook(Color::WHITE, 7, 0));
    board.setPieceAt(Square(1, 0), new Knight(Color::WHITE, 1, 0));
    board.setPieceAt(Square(6, 0), new Knight(Color::WHITE, 6, 0));
    board.setPieceAt(Square(2, 0), new Bishop(Color::WHITE, 2, 0));
    board.setPieceAt(Square(5, 0), new Bishop(Color::WHITE, 5, 0));
    board.setPieceAt(Square(3, 0), new Queen(Color::WHITE, 3, 0));
    board.setPieceAt(Square(4, 0), new King(Color::WHITE, 4, 0));

    // Set up black pieces
    board.setPieceAt(Square(0, 7), new Rook(Color::BLACK, 0, 7));
    board.setPieceAt(Square(7, 7), new Rook(Color::BLACK, 7, 7));
    board.setPieceAt(Square(1, 7), new Knight(Color::BLACK, 1, 7));
    board.setPieceAt(Square(6, 7), new Knight(Color::BLACK, 6, 7));
    board.setPieceAt(Square(2, 7), new Bishop(Color::BLACK, 2, 7));
    board.setPieceAt(Square(5, 7), new Bishop(Color::BLACK, 5, 7));
    board.setPieceAt(Square(3, 7), new Queen(Color::BLACK, 3, 7));
    board.setPieceAt(Square(4, 7), new King(Color::BLACK, 4, 7));

    std::cout << "Expected: Full chess starting position" << std::endl;
    BoardPrinter::print(board);
}

void testAfterOpeningMoves() {
    printTestHeader("TEST 3: After e4 e5 (King's Pawn Opening)");
    Board board;
    
    // Set up pawns
    for (int file = 0; file < 8; file++) {
        board.setPieceAt(Square(file, 1), new Pawn(Color::WHITE, file, 1));
        board.setPieceAt(Square(file, 6), new Pawn(Color::BLACK, file, 6));
    }

    // Set up white pieces
    board.setPieceAt(Square(0, 0), new Rook(Color::WHITE, 0, 0));
    board.setPieceAt(Square(7, 0), new Rook(Color::WHITE, 7, 0));
    board.setPieceAt(Square(1, 0), new Knight(Color::WHITE, 1, 0));
    board.setPieceAt(Square(6, 0), new Knight(Color::WHITE, 6, 0));
    board.setPieceAt(Square(2, 0), new Bishop(Color::WHITE, 2, 0));
    board.setPieceAt(Square(5, 0), new Bishop(Color::WHITE, 5, 0));
    board.setPieceAt(Square(3, 0), new Queen(Color::WHITE, 3, 0));
    board.setPieceAt(Square(4, 0), new King(Color::WHITE, 4, 0));

    // Set up black pieces
    board.setPieceAt(Square(0, 7), new Rook(Color::BLACK, 0, 7));
    board.setPieceAt(Square(7, 7), new Rook(Color::BLACK, 7, 7));
    board.setPieceAt(Square(1, 7), new Knight(Color::BLACK, 1, 7));
    board.setPieceAt(Square(6, 7), new Knight(Color::BLACK, 6, 7));
    board.setPieceAt(Square(2, 7), new Bishop(Color::BLACK, 2, 7));
    board.setPieceAt(Square(5, 7), new Bishop(Color::BLACK, 5, 7));
    board.setPieceAt(Square(3, 7), new Queen(Color::BLACK, 3, 7));
    board.setPieceAt(Square(4, 7), new King(Color::BLACK, 4, 7));

    // Move e2 pawn to e4
    board.setPieceAt(Square(4, 1), nullptr);
    board.setPieceAt(Square(4, 3), new Pawn(Color::WHITE, 4, 3));

    // Move e7 pawn to e5
    board.setPieceAt(Square(4, 6), nullptr);
    board.setPieceAt(Square(4, 4), new Pawn(Color::BLACK, 4, 4));

    std::cout << "Expected: Position after 1.e4 e5" << std::endl;
    BoardPrinter::print(board);
}

void testMiddleGame() {
    printTestHeader("TEST 4: Middle Game Position");
    Board board;
    
    // Scattered pieces in middle game
    board.setPieceAt(Square(4, 4), new King(Color::WHITE, 4, 4));
    board.setPieceAt(Square(4, 7), new King(Color::BLACK, 4, 7));
    
    board.setPieceAt(Square(3, 3), new Queen(Color::WHITE, 3, 3));
    board.setPieceAt(Square(5, 5), new Queen(Color::BLACK, 5, 5));
    
    board.setPieceAt(Square(0, 0), new Rook(Color::WHITE, 0, 0));
    board.setPieceAt(Square(7, 7), new Rook(Color::BLACK, 7, 7));
    
    board.setPieceAt(Square(2, 2), new Bishop(Color::WHITE, 2, 2));
    board.setPieceAt(Square(6, 6), new Bishop(Color::BLACK, 6, 6));
    
    board.setPieceAt(Square(3, 1), new Knight(Color::WHITE, 3, 1));
    board.setPieceAt(Square(5, 6), new Knight(Color::BLACK, 5, 6));
    
    board.setPieceAt(Square(1, 3), new Pawn(Color::WHITE, 1, 3));
    board.setPieceAt(Square(6, 4), new Pawn(Color::BLACK, 6, 4));

    std::cout << "Expected: Scattered pieces simulating middle game" << std::endl;
    BoardPrinter::print(board);
}

void testEndGame() {
    printTestHeader("TEST 5: Endgame - King and Queen vs King");
    Board board;
    
    // Simple endgame: White King and Queen vs Black King
    board.setPieceAt(Square(4, 0), new King(Color::WHITE, 4, 0));
    board.setPieceAt(Square(3, 2), new Queen(Color::WHITE, 3, 2));
    board.setPieceAt(Square(4, 7), new King(Color::BLACK, 4, 7));

    std::cout << "Expected: King + Queen vs King endgame" << std::endl;
    BoardPrinter::print(board);
}

void testPawnPromotion() {
    printTestHeader("TEST 6: Pawn About to Promote");
    Board board;
    
    // White pawn on 7th rank (about to promote)
    board.setPieceAt(Square(0, 6), new Pawn(Color::WHITE, 0, 6));
    board.setPieceAt(Square(4, 0), new King(Color::WHITE, 4, 0));
    
    // Black pawn on 2nd rank (about to promote)
    board.setPieceAt(Square(7, 1), new Pawn(Color::BLACK, 7, 1));
    board.setPieceAt(Square(4, 7), new King(Color::BLACK, 4, 7));

    std::cout << "Expected: Pawns on promotion ranks (7th and 2nd)" << std::endl;
    BoardPrinter::print(board);
}

void testAllPieceTypes() {
    printTestHeader("TEST 7: All Piece Types on One Rank");
    Board board;
    
    // Display all white pieces on rank 1
    board.setPieceAt(Square(0, 1), new King(Color::WHITE, 0, 1));
    board.setPieceAt(Square(1, 1), new Queen(Color::WHITE, 1, 1));
    board.setPieceAt(Square(2, 1), new Rook(Color::WHITE, 2, 1));
    board.setPieceAt(Square(3, 1), new Bishop(Color::WHITE, 3, 1));
    board.setPieceAt(Square(4, 1), new Knight(Color::WHITE, 4, 1));
    board.setPieceAt(Square(5, 1), new Pawn(Color::WHITE, 5, 1));
    
    // Display all black pieces on rank 6
    board.setPieceAt(Square(0, 6), new King(Color::BLACK, 0, 6));
    board.setPieceAt(Square(1, 6), new Queen(Color::BLACK, 1, 6));
    board.setPieceAt(Square(2, 6), new Rook(Color::BLACK, 2, 6));
    board.setPieceAt(Square(3, 6), new Bishop(Color::BLACK, 3, 6));
    board.setPieceAt(Square(4, 6), new Knight(Color::BLACK, 4, 6));
    board.setPieceAt(Square(5, 6), new Pawn(Color::BLACK, 5, 6));

    std::cout << "Expected: All piece types displayed" << std::endl;
    std::cout << "White pieces on rank 2: K Q R B N P" << std::endl;
    std::cout << "Black pieces on rank 7: K Q R B N P" << std::endl;
    BoardPrinter::print(board);
}

void testCheckerboardPattern() {
    printTestHeader("TEST 8: Checkerboard Pattern with Pawns");
    Board board;
    
    // Create checkerboard pattern with alternating pawns
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            if ((rank + file) % 2 == 0) {
                board.setPieceAt(Square(file, rank), new Pawn(Color::WHITE, file, rank));
            } else {
                board.setPieceAt(Square(file, rank), new Pawn(Color::BLACK, file, rank));
            }
        }
    }

    std::cout << "Expected: Checkerboard pattern with alternating white/black pawns" << std::endl;
    BoardPrinter::print(board);
}

void testPieceRenderer() {
    printTestHeader("TEST 9: PieceRenderer Symbol Test");
    
    std::cout << "Testing PieceRenderer::toSymbol() for all pieces:\n" << std::endl;
    
    Pawn whitePawn(Color::WHITE, 0, 0);
    Pawn blackPawn(Color::BLACK, 0, 0);
    Knight whiteKnight(Color::WHITE, 0, 0);
    Knight blackKnight(Color::BLACK, 0, 0);
    Bishop whiteBishop(Color::WHITE, 0, 0);
    Bishop blackBishop(Color::BLACK, 0, 0);
    Rook whiteRook(Color::WHITE, 0, 0);
    Rook blackRook(Color::BLACK, 0, 0);
    Queen whiteQueen(Color::WHITE, 0, 0);
    Queen blackQueen(Color::BLACK, 0, 0);
    King whiteKing(Color::WHITE, 0, 0);
    King blackKing(Color::BLACK, 0, 0);
    
    std::cout << "White Pawn:   " << PieceRenderer::toSymbol(&whitePawn) << std::endl;
    std::cout << "Black Pawn:   " << PieceRenderer::toSymbol(&blackPawn) << std::endl;
    std::cout << "White Knight: " << PieceRenderer::toSymbol(&whiteKnight) << std::endl;
    std::cout << "Black Knight: " << PieceRenderer::toSymbol(&blackKnight) << std::endl;
    std::cout << "White Bishop: " << PieceRenderer::toSymbol(&whiteBishop) << std::endl;
    std::cout << "Black Bishop: " << PieceRenderer::toSymbol(&blackBishop) << std::endl;
    std::cout << "White Rook:   " << PieceRenderer::toSymbol(&whiteRook) << std::endl;
    std::cout << "Black Rook:   " << PieceRenderer::toSymbol(&blackRook) << std::endl;
    std::cout << "White Queen:  " << PieceRenderer::toSymbol(&whiteQueen) << std::endl;
    std::cout << "Black Queen:  " << PieceRenderer::toSymbol(&blackQueen) << std::endl;
    std::cout << "White King:   " << PieceRenderer::toSymbol(&whiteKing) << std::endl;
    std::cout << "Black King:   " << PieceRenderer::toSymbol(&blackKing) << std::endl;
    std::cout << "Null piece:   '" << PieceRenderer::toSymbol(nullptr) << "'" << std::endl;
}

void testCornerPieces() {
    printTestHeader("TEST 10: Pieces in All Four Corners");
    Board board;
    
    // Place pieces in all four corners
    board.setPieceAt(Square(0, 0), new Rook(Color::WHITE, 0, 0));   // a1
    board.setPieceAt(Square(7, 0), new Rook(Color::WHITE, 7, 0));   // h1
    board.setPieceAt(Square(0, 7), new Rook(Color::BLACK, 0, 7));   // a8
    board.setPieceAt(Square(7, 7), new Rook(Color::BLACK, 7, 7));   // h8
    
    // Add kings in center
    board.setPieceAt(Square(4, 3), new King(Color::WHITE, 4, 3));   // e4
    board.setPieceAt(Square(4, 4), new King(Color::BLACK, 4, 4));   // e5

    std::cout << "Expected: Rooks in all four corners, Kings in center" << std::endl;
    BoardPrinter::print(board);
}

int main() {
    std::cout << "============================================" << std::endl;
    std::cout << "   BOARD PRINTER & PIECE RENDERER TESTS" << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << "\nThese tests verify the BoardPrinter displays:" << std::endl;
    std::cout << "  - Correct square colors (alternating pattern)" << std::endl;
    std::cout << "  - Correct piece symbols (Unicode chess pieces)" << std::endl;
    std::cout << "  - Correct piece colors (white/black)" << std::endl;
    std::cout << "  - Proper board coordinates (files a-h, ranks 1-8)" << std::endl;

    // Run all tests
    testEmptyBoard();
    testInitialPosition();
    testAfterOpeningMoves();
    testMiddleGame();
    testEndGame();
    testPawnPromotion();
    testAllPieceTypes();
    testCheckerboardPattern();
    testPieceRenderer();
    testCornerPieces();

    // Summary
    std::cout << "\n========================================" << std::endl;
    std::cout << "   ALL TESTS COMPLETED" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "\nVisual Verification Checklist:" << std::endl;
    std::cout << "  ✓ Board has alternating light/dark squares" << std::endl;
    std::cout << "  ✓ White pieces appear in bright white" << std::endl;
    std::cout << "  ✓ Black pieces appear in dark gray" << std::endl;
    std::cout << "  ✓ Unicode symbols render correctly" << std::endl;
    std::cout << "  ✓ File labels (a-h) appear at top and bottom" << std::endl;
    std::cout << "  ✓ Rank labels (1-8) appear on both sides" << std::endl;
    std::cout << "  ✓ Pieces appear in correct positions" << std::endl;

    return 0;
}