#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.h"

class Knight : public Piece
{
public:
    Knight(Color color) : Piece(color, PieceType::KNIGHT) {}

    std::unordered_set<Position, PositionHash> getLegalMoves(Position currentPos, Board *board) override;
};

#endif