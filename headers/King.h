#ifndef KING_H
#define KING_H

#include "Piece.h"

class King : public Piece
{
private:
    bool hasMoved = false;

public:
    King(Color color) : Piece(color, PieceType::KING) {}

    std::unordered_set<Position, PositionHash> getLegalMoves(Position currentPos, Board *board) override;
    bool getHasMoved() const;
    void markAsMoved();
};

#endif