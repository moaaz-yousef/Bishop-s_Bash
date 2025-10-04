#include <unordered_set>
#include "../headers/Piece.h"
#include "../headers/Types.h"

class Board;

Piece::Piece(Color color, PieceType type)
{
    this->color = color;
    this->pieceType = type;
}

Color Piece::getColor()
{
    return this->color;
}

PieceType Piece::getPieceType()
{
    return this->pieceType;
}
