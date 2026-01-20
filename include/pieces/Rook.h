class Rook : public Piece {
public:
    Rook(Color color, int file, int rank);

    std::vector<Move> getLegalMoves(const Board& board) const override;
    Piece* clone() const override;
};
