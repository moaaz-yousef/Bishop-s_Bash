#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"

class Rook : public Piece
{
private:
    bool hasMoved = false;

public:
    Rook(Color color) : Piece(color, PieceType::ROOK) {}

    std::unordered_set<Position, PositionHash> getLegalMoves(Position currentPos, Board *board) override;
    bool getHasMoved() const;
    void markAsMoved();
};

#endif