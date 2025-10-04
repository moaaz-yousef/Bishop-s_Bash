#ifndef PIECE_H
#define PIECE_H

#include "Types.h"
#include "Board.h"
#include <unordered_set>

class Board;

class Piece
{
private:
    Color color;
    PieceType pieceType;

public:
    Piece(Color color, PieceType pieceType);
    Color getColor();
    PieceType getPieceType();
    virtual std::unordered_set<Position, PositionHash> getLegalMoves(Position currentPos, Board *board) = 0;
};

#endif