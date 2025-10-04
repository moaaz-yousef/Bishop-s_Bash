#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.h"

class Queen : public Piece
{
public:
    Queen(Color color) : Piece(color, PieceType::QUEEN) {}

    std::unordered_set<Position, PositionHash> getLegalMoves(Position currentPos, Board *board) override;
};

#endif