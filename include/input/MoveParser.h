#pragma once

#include <string>
#include <optional>
#include "board/Move.h"
#include "board/Board.h"
#include "../pieces/Piece.h"

struct ParsedMove {
    std::optional<Move> move;
    bool isValid = false;
    bool isCapture = false;
    bool isCheck = false;
    bool isCastling = false;

    ParsedMove() = default;
    ParsedMove(std::optional<Move> m, bool valid, bool capture, bool check, bool castling)
        : move(m), isValid(valid), isCapture(capture), isCheck(check), isCastling(castling) {}
};

class MoveParser {
public:
    static ParsedMove parse(const std::string& input, const Board& board, Color turn);
    static ParsedMove parseSimple(const std::string& input, const Board& board, Color turn);
    static ParsedMove parseAlgebraic(const std::string& input, const Board& board, Color turn);

private:
    static std::optional<Move> resolveAlgebraic(
        const std::string& token,
        const Board& board,
        Color turn,
        bool& isCapture,
        bool& isCheck,
        bool& isCastling,
        std::optional<PieceType>& promotion
    );

    static std::vector<Piece*> getCandidates(
        PieceType type,
        const Board& board,
        Color turn,
        const Square& target
    );
};