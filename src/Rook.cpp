#include "../headers/Rook.h"

bool Rook::getHasMoved() const
{
    return hasMoved;
}

void Rook::markAsMoved()
{
    hasMoved = true;
}

std::unordered_set<Position, PositionHash> Rook::getLegalMoves(Position currentPos, Board *board)
{
    std::unordered_set<Position, PositionHash> legalMoves;
    int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    for (auto &direction : directions)
    {
        Position pos = currentPos;
        while (true)
        {
            pos.row += direction[0];
            pos.col += direction[1];

            if (pos.row < 0 || pos.row >= 8 || pos.col < 0 || pos.col >= 8)
                break;

            if (board->hasPiece(pos))
            {
                Piece *targetPiece = board->getPieceAt(pos);
                if (targetPiece->getColor() != getColor() && board->isSafeMove({currentPos, pos, this}))
                    legalMoves.insert(pos);
                break;
            }
            else
            {
                if (board->isSafeMove({currentPos, pos, this}))
                    legalMoves.insert(pos);
            }
        }
    }

    return legalMoves;
}