#include "input/MoveParser.h"
#include <vector>
#include <cctype>

ParsedMove MoveParser::parse(const std::string& input, const Board& board, Color turn) {
    if (input.empty()) return ParsedMove(std::nullopt, false, false, false, false);

    if (input.find('x') != std::string::npos || input.find('O') != std::string::npos ||
        std::isupper(input[0]) || std::isdigit(input[0])) {
        return parseAlgebraic(input, board, turn);
    }

    return parseSimple(input, board, turn);
}

ParsedMove MoveParser::parseSimple(const std::string& input, const Board& board, Color turn) {
    if (input.size() < 4) return ParsedMove(std::nullopt, false, false, false, false);

    Square from(0,0), to(0,0);
    if (!Square::fromString(input.substr(0,2), from)) return ParsedMove(std::nullopt, false, false, false, false);
    if (!Square::fromString(input.substr(2,2), to)) return ParsedMove(std::nullopt, false, false, false, false);

    Move move(from, to);
    bool valid = board.isLegalMove(move, turn);
    return ParsedMove(valid ? std::optional<Move>(move) : std::nullopt, valid, !board.isEmpty(to.getFile(), to.getRank()), false, false);
}

ParsedMove MoveParser::parseAlgebraic(const std::string& input, const Board& board, Color turn) {
    bool isCapture = false;
    bool isCheck = false;
    bool isCastling = false;
    std::optional<PieceType> promotion = std::nullopt;

    std::string token;
    for (char c : input) {
        if (c == '+' || c == '#') { isCheck = true; continue; }
        if (c == 'x') { isCapture = true; continue; }
        token.push_back(c);
    }

    if (token == "O-O" || token == "0-0") {
        isCastling = true;
        Move m = board.getCastlingMove(turn, true);
        bool valid = board.isLegalMove(m, turn);
        return ParsedMove(valid ? std::optional<Move>(m) : std::nullopt, valid, false, isCheck, true);
    }
    if (token == "O-O-O" || token == "0-0-0") {
        isCastling = true;
        Move m = board.getCastlingMove(turn, false);
        bool valid = board.isLegalMove(m, turn);
        return ParsedMove(valid ? std::optional<Move>(m) : std::nullopt, valid, false, isCheck, true);
    }

    if (token.size() >= 4 && token[token.size()-2] == '=') {
        char prom = token.back();
        promotion = charToPieceType(prom);
        token = token.substr(0, token.size() - 2);
    }

    auto optMove = resolveAlgebraic(token, board, turn, isCapture, isCheck, isCastling, promotion);
    if (!optMove.has_value()) return ParsedMove(std::nullopt, false, false, false, false);

    Move m = optMove.value();
    bool valid = board.isLegalMove(m, turn);
    return ParsedMove(valid ? std::optional<Move>(m) : std::nullopt, valid, isCapture, isCheck, isCastling);
}

std::optional<Move> MoveParser::resolveAlgebraic(
    const std::string& token,
    const Board& board,
    Color turn,
    bool& isCapture,
    bool& isCheck,
    bool& isCastling,
    std::optional<PieceType>& promotion
) {
    if (token.empty()) return std::nullopt;

    PieceType type = PieceType::PAWN;
    int idx = 0;

    if (std::isupper(token[0]) && token[0] != 'O') {
        auto optType = charToPieceType(token[0]);
        if (optType) {
            type = *optType;
        }
        idx++;
    }

    std::string disamb;
    std::string targetStr;

    for (; idx < (int)token.size(); idx++) {
        char c = token[idx];
        if (c == 'x') continue;
        if (c >= 'a' && c <= 'h') {
            if (idx + 1 < (int)token.size() && std::isdigit(token[idx + 1])) {
                targetStr = token.substr(idx, 2);
                break;
            } else {
                disamb.push_back(c);
            }
        } else if (std::isdigit(c)) {
            disamb.push_back(c);
        }
    }

    if (targetStr.empty()) return std::nullopt;

    Square target(0,0);
    if (!Square::fromString(targetStr, target)) return std::nullopt;

    std::vector<Piece*> candidates = getCandidates(type, board, turn, target);
    if (candidates.empty()) return std::nullopt;

    std::vector<Piece*> filtered;
    for (Piece* p : candidates) {
        Square from(p->getFile(), p->getRank());
        Move m(from, target, promotion);
        if (!board.isLegalMove(m, turn)) continue;

        if (!disamb.empty()) {
            bool match = false;
            if (disamb.size() == 1) {
                if (disamb[0] >= 'a' && disamb[0] <= 'h') match = (from.getFile() == disamb[0] - 'a');
                if (disamb[0] >= '1' && disamb[0] <= '8') match = (from.getRank() == disamb[0] - '1');
            } else if (disamb.size() == 2) {
                Square s(0,0);
                if (Square::fromString(disamb, s)) match = (from == s);
            }
            if (!match) continue;
        }

        filtered.push_back(p);
    }

    if (filtered.size() != 1) return std::nullopt;

    Piece* p = filtered[0];
    Square from(p->getFile(), p->getRank());
    return Move(from, target, promotion);
}

std::vector<Piece*> MoveParser::getCandidates(
    PieceType type,
    const Board& board,
    Color turn,
    const Square& target
) {
    std::vector<Piece*> candidates;
    for (Piece* p : board.getPieces(turn)) {
        if (p->getType() != type) continue;
        for (const Move& m : p->getLegalMoves(board)) {
            if (m.getTo() == target) {
                candidates.push_back(p);
                break;
            }
        }
    }
    return candidates;
}