#ifndef TYPES_H
#define TYPES_H

#include <cstddef>

enum Color
{
    WHITE,
    BLACK
};

enum PieceType
{
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING
};

struct Position
{
    int row;
    int col;

    bool operator==(const Position &other) const
    {
        return row == other.row && col == other.col;
    }
};

struct PositionHash
{
    size_t operator()(const Position &p) const noexcept
    {
        return p.row * 8 + p.col;
    }
};

class Piece;

struct Move
{
    Position from;
    Position to;
    Piece *movedPiece;
};
#endif