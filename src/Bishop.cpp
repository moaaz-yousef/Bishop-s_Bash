#include "../headers/Bishop.h"

std::unordered_set<Position, PositionHash> Bishop::getLegalMoves(Position currentPos, Board *board)
{
    std::unordered_set<Position, PositionHash> legalMoves;
    int directions[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
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
            else if (board->isSafeMove({currentPos, pos, this}))
                legalMoves.insert(pos);
        }
    }

    return legalMoves;
}