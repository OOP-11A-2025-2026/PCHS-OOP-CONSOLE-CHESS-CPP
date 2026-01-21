#include <iostream>
#include <fstream>
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
#include "game/Game.h"
#include "input/PGNHandler.h"

void printTestHeader(const std::string& testName) {
    std::cout << "\n========================================" << std::endl;
    std::cout << "  " << testName << std::endl;
    std::cout << "========================================" << std::endl;
}

void testSelfCheckLogic() {
    printTestHeader("TEST: Self Check Validation (Darian 2.2)");
    Game game; 
    Board& board = game.getBoard();
    
    // Clear board for custom setup
    for(int f=0; f<8; f++) for(int r=0; r<8; r++) board.removePieceAt(f, r);
    
    // Setup: White King e1, Black Rook e8 (Open file).
    board.placePiece(new King(Color::WHITE, 4, 0)); // e1
    board.placePiece(new Rook(Color::BLACK, 4, 7)); // e8
    // Add a White Rook at d1 to move.
    board.placePiece(new Rook(Color::WHITE, 3, 0)); // d1
    
    std::cout << "Scenario: White King e1, Black Rook e8. White Rook d1." << std::endl;
    BoardPrinter::print(board);
    
    // Move 1: d1 -> d2 (Rook). King stays on e1 under attack.
    Move badMove(Square(3, 0), Square(3, 1));
    bool result = game.makeMove(badMove);
    std::cout << "Attempt d1-d2 (ignores check): " << (result ? "ALLOWED (FAIL)" : "REJECTED (PASS)") << std::endl;
    
    // Move 2: e1 -> f1 (King). Escapes check.
    Move goodMove(Square(4, 0), Square(5, 0));
    
    bool result2 = game.makeMove(goodMove);
    std::cout << "Attempt e1-f1 (escapes check): " << (result2 ? "ALLOWED (PASS)" : "REJECTED (FAIL)") << std::endl;
}

void testPGNHandler() {
    printTestHeader("TEST: PGN Save/Load (Darian 6)");
    /*
    // Old test 
    Game game;
    // Play Fool's Mate: 1. f3 e5 2. g4 Qh4#
    game.makeMove(Move(Square(5, 1), Square(5, 2))); // f2-f3
    game.makeMove(Move(Square(4, 6), Square(4, 4))); // e7-e5
    game.makeMove(Move(Square(6, 1), Square(6, 3))); // g2-g4
    
    std::cout << "Game played (3 moves). Saving to 'test_game.pgn'..." << std::endl;
    PGNHandler::saveToFile(game, "test_game.pgn");
    */
    
    // CRAZY PGN TEST: Castling (Short & Long), En Passant, Promotion, Checkmate
    std::string crazyPGN = R"(
[Event "Crazy Test"]
[Result "1-0"]

1. e4 d5 2. e5 f5 3. exf6 Kd7 4. fxg7 Qe8 5. gxh8=Q h5 6. Qf3 Nf6
7. Bb5+ Nc6 8. Ne2 a6 9. O-O axb5 10. d4 b4 11. c3 b3 12. a3 b5
13. Be3 Bb7 14. Nd2 Kc8 15. Nxb3 Qf7 16. Nc5 e5 17. Qf5+ Kb8
18. Qxf6 Bxc5 19. Qxf7 exd4 20. cxd4 Ka7 21. dxc5 d4 22. Qxc7 dxe3
23. Qb6+ Kb8 24. Nd4 Nxd4 25. Qd8+ Ka7 26. Qxd4 h4 27. c6+ Kb8
28. Qd8+ Bc8 29. Qb6+ Bb7 30. Qxb7# 1-0
)";

    // En Passant happens at 3. exf6 (f5 was move 2 black)
    // Promotion happens at 5. gxh8=Q
    // Short Castle White at 9. O-O
    // Long Castle is ... well, the king moves manually to c8 then Ka7/Kb8, 
    // maybe standard castling didn't trigger in this silly sequence, but logic is tested implicitly by King handling.
    
    // Let's ensure we test O-O-O specifically.
    // 1. e4 d6 2. Nc3 Qd7 3. b3 Nc6 4. Bb2 b6 5. Qe2 Bb7 6. O-O-O O-O-O
    
    std::string crazyPGN2 = R"(
[Event "Castling Test"]
[Result "*"]

1. e4 d5 2. Nc3 Nc6 3. d4 Qd6 4. Be3 Bd7 5. Qd2 O-O-O 6. O-O-O e5
7. dxe5 Qxe5 8. exd5 Bg4 9. Be2 Nf6 10. f3 Bh5 11. f4 Qf5 12. Bd3 Qg4 13. dxc6
)";

    std::ofstream outfile("crazy.pgn");
    outfile << crazyPGN;
    outfile.close();

    std::cout << "Created 'crazy.pgn' with En Passant, Promotion, Short Castle, Checkmate." << std::endl;
    
    Game loadedGame;
    std::cout << "Loading 'crazy.pgn'..." << std::endl;
    PGNHandler::loadFromFile(loadedGame, "crazy.pgn");
    
    std::cout << "Loaded Game Last Move: " << (loadedGame.getBoard().getLastMove() ? "Exists" : "Null") << std::endl;
    BoardPrinter::print(loadedGame.getBoard());
    
    auto history = loadedGame.getMoveHistory();
    std::cout << "History size: " << history.size() << std::endl; 
    
    if (history.size() > 0) {
        std::cout << "Last move stored: " << history.back() << std::endl;
    }

    std::cout << "Verifying Board State (Checkmate)..." << std::endl;
    if (loadedGame.getState() == GameState::CHECKMATE) {
        std::cout << "Game State: CHECKMATE (PASS)" << std::endl;
    } else {
        std::cout << "Game State: " << (int)loadedGame.getState() << " (Expected CHECKMATE)" << std::endl;
    }

    // Test 2: Double Castling
    std::ofstream outfile2("castling.pgn");
    outfile2 << crazyPGN2;
    outfile2.close();
    
    std::cout << "\nLoading 'castling.pgn' (Testing O-O-O)..." << std::endl;
    Game castlingGame;
    PGNHandler::loadFromFile(castlingGame, "castling.pgn");
    BoardPrinter::print(castlingGame.getBoard());
    std::cout << "History size: " << castlingGame.getMoveHistory().size() << " (Expected 13 white moves, 12 black)" << std::endl;

}

// Rename main to something else if you link this file with the real main
int runDarianTests() {
    testSelfCheckLogic();
    testPGNHandler();
    return 0;
}
