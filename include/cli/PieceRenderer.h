#ifndef PIECERENDERER_H
#define PIECERENDERER_H

#include <string>

class Piece;

/**
 * Utility class for rendering chess pieces as Unicode symbols.
 */
class PieceRenderer {
public:
    /**
     * Converts a piece to its Unicode chess symbol.
     * @param piece The piece to convert
     * @return Unicode string representing the piece (e.g., "♔", "♕", "♖", "♗", "♘", "♙")
     */
    static std::string toSymbol(const Piece* piece);
};

#endif // PIECERENDERER_H