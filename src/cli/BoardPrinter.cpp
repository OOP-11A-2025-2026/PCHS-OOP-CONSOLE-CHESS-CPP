#include "cli/BoardPrinter.h"
#include "cli/PieceRenderer.h"
#include "board/Board.h"
#include "board/Square.h"
#include "pieces/Piece.h"
#include "enums/Color.h"
#include <iostream>

// Define static ANSI color codes
const char* BoardPrinter::RESET = "\033[0m";
const char* BoardPrinter::WHITE_BG = "\033[47m";    // white background
const char* BoardPrinter::BLACK_BG = "\033[40m";    // dark gray background
const char* BoardPrinter::WHITE_PIECE = "\033[97m"; // bright white text
const char* BoardPrinter::BLACK_PIECE = "\033[90m"; // dark gray text

/**
 * Prints the chess board to the console with colored squares and pieces.
 */
void BoardPrinter::print(const Board& board) {
    std::cout << "\n   a  b  c  d  e  f  g  h" << std::endl;

    for (int rank = 7; rank >= 0; rank--) {
        std::cout << " " << (rank + 1) << " ";

        for (int file = 0; file < 8; file++) {
            bool lightSquare = (rank + file) % 2 == 0;
            const char* bg = lightSquare ? WHITE_BG : BLACK_BG;

            Square square(file, rank);
            Piece* piece = board.getPieceAt(square);

            if (piece == nullptr) {
                std::cout << bg << "   " << RESET;
            } else {
                const char* fg = piece->getColor() == Color::WHITE
                                ? WHITE_PIECE
                                : BLACK_PIECE;

                std::string symbol = PieceRenderer::toSymbol(piece);
                std::cout << bg << fg << " " << symbol << " " << RESET;
            }
        }

        std::cout << " " << (rank + 1) << std::endl;
    }

    std::cout << "   a  b  c  d  e  f  g  h\n" << std::endl;
}