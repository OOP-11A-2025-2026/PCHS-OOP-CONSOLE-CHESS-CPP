#include "input/MoveParser.h"
#include <vector>
#include <cctype>

ParsedMove MoveParser::parse(const std::string& input, const Board& board, Color turn) {
    if (input.empty()) return ParsedMove(std::nullopt, false, false, false, false);

    // Remove whitespace from input for easier parsing
    std::string cleanInput;
    for (char c : input) {
        if (!std::isspace(c)) {
            cleanInput.push_back(c);
        }
    }

    // If input is exactly 4 or 5 characters and looks like coordinate notation (e.g. e2e4 or e7e8q)
    // and starts with file-rank-file-rank, treat as Simple.
    // Otherwise treat as Algebraic.
    // ...
    
    bool looksLikeCoordinate = false;
    if (cleanInput.length() == 4 || cleanInput.length() == 5) {
        if (cleanInput[0] >= 'a' && cleanInput[0] <= 'h' &&
            cleanInput[1] >= '1' && cleanInput[1] <= '8' &&
            cleanInput[2] >= 'a' && cleanInput[2] <= 'h' &&
            cleanInput[3] >= '1' && cleanInput[3] <= '8') {
            looksLikeCoordinate = true;
        }
    }

    if (looksLikeCoordinate) {
         return parseSimple(cleanInput, board, turn);
    }
    
    return parseAlgebraic(cleanInput, board, turn);
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

    // A better approach: 
    // The target square is ALWAYS the last 2 characters of the token (e.g. "e4", "f6", "h8").
    // Anything before that (after the piece letter) is disambiguation.
    // CAUTION: This assumes the token is stripped of 'x', '+', '#', '='.
    // "exf6" -> token is "ef6". Target "f6". Disamb "e".
    // "Nbd7" -> token "Nbd7". Piece 'N'. Disamb "b". Target "d7".
    // "R1e2" -> token "R1e2". Piece 'R'. Disamb "1". Target "e2".
    
    if (token.size() >= 2 + idx) {
        // Check if the last two chars form a square
        if (std::isdigit(token.back()) && 
            token[token.size()-2] >= 'a' && token[token.size()-2] <= 'h') {
            
            targetStr = token.substr(token.size()-2);
            if (token.size() > 2 + idx) {
                // There is disambiguation info in between
                disamb = token.substr(idx, token.size() - 2 - idx);
            }
        }
    } else {
        // Fallback or error
        return std::nullopt;
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
        
        bool isLegal = board.isLegalMove(m, turn);
        if (!isLegal) {
             // std::cout << "DEBUG: Candidate " << (char)('a'+from.getFile()) << from.getRank()+1 
             //           << " -> " << (char)('a'+target.getFile()) << target.getRank()+1 
             //           << " is NOT legal." << std::endl;
             continue;
        }

        if (!disamb.empty()) {
            bool match = false;
            // ...
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