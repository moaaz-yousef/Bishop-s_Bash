#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"

class Pawn : public Piece
{
public:
    Pawn(Color color) : Piece(color, PieceType::PAWN) {}

    std::unordered_set<Position, PositionHash> getLegalMoves(Position currentPos, Board *board) override;
};

#endif