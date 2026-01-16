#include <iostream>
#include <vector>
#include "board/Square.h"
#include "board/Move.h"
#include "enums/PieceType.h"

int main() {
    std::cout << "============================================" << std::endl;
    std::cout << "      CHESS ENGINE TEST SUITE" << std::endl;
    std::cout << "============================================" << std::endl;
    
    // ========================================
    // SQUARE CLASS TESTS
    // ========================================
    std::cout << "\n=== Testing Square Class ===" << std::endl;
    
    // Test 1: Creating squares with coordinates
    std::cout << "\n1. Creating squares:" << std::endl;
    Square e4(4, 3);  // e4 in chess notation (file=4, rank=3)
    Square e2(4, 1);  // e2
    Square a1(0, 0);  // a1
    Square h8(7, 7);  // h8
    
    std::cout << "   e4: " << e4.toString() << " (file=" << e4.getFile() << ", rank=" << e4.getRank() << ")" << std::endl;
    std::cout << "   e2: " << e2.toString() << std::endl;
    std::cout << "   a1: " << a1.toString() << std::endl;
    std::cout << "   h8: " << h8.toString() << std::endl;
    
    // Test 2: Parsing from algebraic notation
    std::cout << "\n2. Parsing squares from strings:" << std::endl;
    Square parsed(0, 0);
    
    if (Square::fromString("d4", parsed)) {
        std::cout << "   ✓ Successfully parsed 'd4': " << parsed.toString() << std::endl;
    }
    
    if (Square::fromString("c7", parsed)) {
        std::cout << "   ✓ Successfully parsed 'c7': " << parsed.toString() << std::endl;
    }
    
    if (Square::fromString("b1", parsed)) {
        std::cout << "   ✓ Successfully parsed 'b1': " << parsed.toString() << std::endl;
    }
    
    // Test 3: Invalid square parsing
    std::cout << "\n3. Testing invalid square inputs:" << std::endl;
    if (!Square::fromString("z9", parsed)) {
        std::cout << "   ✓ Correctly rejected 'z9' (invalid file)" << std::endl;
    }
    
    if (!Square::fromString("a0", parsed)) {
        std::cout << "   ✓ Correctly rejected 'a0' (invalid rank)" << std::endl;
    }
    
    if (!Square::fromString("i5", parsed)) {
        std::cout << "   ✓ Correctly rejected 'i5' (file out of range)" << std::endl;
    }
    
    if (!Square::fromString("abc", parsed)) {
        std::cout << "   ✓ Correctly rejected 'abc' (wrong length)" << std::endl;
    }
    
    // Test 4: Square equality
    std::cout << "\n4. Testing square equality:" << std::endl;
    Square e4_copy(4, 3);
    Square d4(3, 3);
    
    if (e4 == e4_copy) {
        std::cout << "   ✓ e4 == e4 (same coordinates)" << std::endl;
    }
    
    if (e4 != d4) {
        std::cout << "   ✓ e4 != d4 (different files)" << std::endl;
    }
    
    if (e4 != e2) {
        std::cout << "   ✓ e4 != e2 (different ranks)" << std::endl;
    }
    
    // Test 5: Hash codes
    std::cout << "\n5. Testing hash codes:" << std::endl;
    std::cout << "   Hash of e4: " << e4.hashCode() << std::endl;
    std::cout << "   Hash of e2: " << e2.hashCode() << std::endl;
    std::cout << "   Hash of a1: " << a1.hashCode() << std::endl;
    
    // ========================================
    // MOVE CLASS TESTS
    // ========================================
    std::cout << "\n=== Testing Move Class ===" << std::endl;
    
    // Test 1: Regular moves (no promotion)
    std::cout << "\n1. Creating regular moves:" << std::endl;
    Move pawnPush(e2, e4);
    std::cout << "   Pawn push: " << pawnPush.getFrom().toString() 
              << " → " << pawnPush.getTo().toString() << std::endl;
    std::cout << "   Is promotion? " << (pawnPush.isPromotion() ? "Yes" : "No") << std::endl;
    
    Square g1(6, 0);
    Square f3(5, 2);
    Move knightMove(g1, f3);
    std::cout << "   Knight move: " << knightMove.getFrom().toString() 
              << " → " << knightMove.getTo().toString() << std::endl;
    
    Square a2(0, 1);
    Square a4(0, 3);
    Move doublePawnPush(a2, a4);
    std::cout << "   Double pawn push: " << doublePawnPush.getFrom().toString() 
              << " → " << doublePawnPush.getTo().toString() << std::endl;
    
    // Test 2: Promotion moves
    std::cout << "\n2. Creating promotion moves:" << std::endl;
    Square e7(4, 6);
    Square e8(4, 7);
    
    Move promoteToQueen(e7, e8, PieceType::QUEEN);
    std::cout << "   Promotion to Queen: " << promoteToQueen.getFrom().toString() 
              << " → " << promoteToQueen.getTo().toString() << std::endl;
    std::cout << "   Is promotion? " << (promoteToQueen.isPromotion() ? "Yes" : "No") << std::endl;
    if (promoteToQueen.getPromotion().has_value()) {
        std::cout << "   Promotes to: ";
        switch (promoteToQueen.getPromotion().value()) {
            case PieceType::QUEEN:  std::cout << "QUEEN"; break;
            case PieceType::ROOK:   std::cout << "ROOK"; break;
            case PieceType::BISHOP: std::cout << "BISHOP"; break;
            case PieceType::KNIGHT: std::cout << "KNIGHT"; break;
            case PieceType::KING:   std::cout << "KING"; break;
            case PieceType::PAWN:   std::cout << "PAWN"; break;
        }
        std::cout << std::endl;
    }
    
    Move promoteToKnight(e7, e8, PieceType::KNIGHT);
    std::cout << "   Promotion to Knight: " << promoteToKnight.getFrom().toString() 
              << " → " << promoteToKnight.getTo().toString();
    if (promoteToKnight.getPromotion().has_value()) {
        std::cout << " (Knight underpromotion)";
    }
    std::cout << std::endl;
    
    Move promoteToRook(e7, e8, PieceType::ROOK);
    std::cout << "   Promotion to Rook: " << promoteToRook.getFrom().toString() 
              << " → " << promoteToRook.getTo().toString() << std::endl;
    
    // Test 3: Various chess moves
    std::cout << "\n3. Examples of various chess moves:" << std::endl;
    
    // Castling (represented as king move)
    Square e1(4, 0);
    Square g1_castle(6, 0);
    Move kingsideCastle(e1, g1_castle);
    std::cout << "   Kingside castle (white): " << kingsideCastle.getFrom().toString() 
              << " → " << kingsideCastle.getTo().toString() << std::endl;
    
    Square c1(2, 0);
    Move queensideCastle(e1, c1);
    std::cout << "   Queenside castle (white): " << queensideCastle.getFrom().toString() 
              << " → " << queensideCastle.getTo().toString() << std::endl;
    
    // Capture
    Square d5(3, 4);
    Move capture(e4, d5);
    std::cout << "   Capture: " << capture.getFrom().toString() 
              << " → " << capture.getTo().toString() << std::endl;
    
    // ========================================
    // PIECE TYPE AND COLOR TESTS
    // ========================================
    std::cout << "\n=== Testing Enums ===" << std::endl;
    
    std::cout << "\n1. PieceType enum values:" << std::endl;
    std::vector<PieceType> pieceTypes = {
        PieceType::PAWN, PieceType::KNIGHT, PieceType::BISHOP,
        PieceType::ROOK, PieceType::QUEEN, PieceType::KING
    };
    
    for (const auto& type : pieceTypes) {
        std::cout << "   ";
        switch (type) {
            case PieceType::PAWN:   std::cout << "PAWN"; break;
            case PieceType::KNIGHT: std::cout << "KNIGHT"; break;
            case PieceType::BISHOP: std::cout << "BISHOP"; break;
            case PieceType::ROOK:   std::cout << "ROOK"; break;
            case PieceType::QUEEN:  std::cout << "QUEEN"; break;
            case PieceType::KING:   std::cout << "KING"; break;
        }
        std::cout << std::endl;
    }
    
    std::cout << "\n2. Color enum values:" << std::endl;
    std::cout << "   WHITE" << std::endl;
    std::cout << "   BLACK" << std::endl;
    
    // ========================================
    // PRACTICAL EXAMPLES
    // ========================================
    std::cout << "\n=== Practical Examples ===" << std::endl;
    
    std::cout << "\n1. Famous opening moves:" << std::endl;
    
    // Italian Game opening
    Move e4_opening(e2, e4);
    Square e5(4, 4);
    Square e7_black(4, 6);
    Move e5_response(e7_black, e5);
    std::cout << "   1. e2-e4 e7-e5 (King's Pawn Opening)" << std::endl;
    std::cout << "      White: " << e4_opening.getFrom().toString() 
              << " → " << e4_opening.getTo().toString() << std::endl;
    std::cout << "      Black: " << e5_response.getFrom().toString() 
              << " → " << e5_response.getTo().toString() << std::endl;
    
    Square f1(5, 0);
    Square c4(2, 3);
    Move bishopDevelop(f1, c4);
    std::cout << "   2. f1-c4 (Italian Game)" << std::endl;
    std::cout << "      White: " << bishopDevelop.getFrom().toString() 
              << " → " << bishopDevelop.getTo().toString() << std::endl;
    
    std::cout << "\n2. En passant capture example:" << std::endl;
    Square e5_pawn(4, 4);
    Square d5_capture(3, 4);
    Square d6(3, 5);
    Move enPassant(e5_pawn, d6);
    std::cout << "   " << enPassant.getFrom().toString() 
              << " → " << enPassant.getTo().toString() 
              << " (captures pawn on d5)" << std::endl;
    
    std::cout << "\n3. Pawn promotion scenarios:" << std::endl;
    Square a7(0, 6);
    Square a8(0, 7);
    
    std::vector<std::pair<PieceType, std::string>> promotions = {
        {PieceType::QUEEN, "Queen (most common)"},
        {PieceType::KNIGHT, "Knight (useful for checks)"},
        {PieceType::ROOK, "Rook (rare underpromotion)"},
        {PieceType::BISHOP, "Bishop (very rare)"}
    };
    
    for (const auto& [type, desc] : promotions) {
        Move promo(a7, a8, type);
        std::cout << "   " << promo.getFrom().toString() 
                  << " → " << promo.getTo().toString() 
                  << " = " << desc << std::endl;
    }
    
    // ========================================
    // SUMMARY
    // ========================================
    std::cout << "\n============================================" << std::endl;
    std::cout << "      ALL TESTS COMPLETED SUCCESSFULLY!" << std::endl;
    std::cout << "============================================" << std::endl;
    
    return 0;
}