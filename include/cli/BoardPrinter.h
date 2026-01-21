#ifndef BOARDPRINTER_H
#define BOARDPRINTER_H

class Board;

/**
 * Utility class for printing the chess board to the console with colors.
 */
class BoardPrinter {
private:
    // ANSI color codes
    static const char* RESET;
    static const char* WHITE_BG;
    static const char* BLACK_BG;
    static const char* WHITE_PIECE;
    static const char* BLACK_PIECE;

    // Private constructor to prevent instantiation
    BoardPrinter() = delete;

public:
    /**
     * Prints the chess board to the console with colored squares and pieces.
     * @param board The board to print
     */
    static void print(const Board& board);
};

#endif // BOARDPRINTER_H