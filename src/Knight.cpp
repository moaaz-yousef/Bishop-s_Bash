#include "../headers/Knight.h"

std::unordered_set<Position, PositionHash> Knight::getLegalMoves(Position currentPos, Board *board)
{
    std::unordered_set<Position, PositionHash> legalMoves;
    int moves[8][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};
    for (auto &move : moves)
    {
        Position newPos = {currentPos.row + move[0], currentPos.col + move[1]};
        if (newPos.row >= 0 && newPos.row < 8 && newPos.col >= 0 && newPos.col < 8)
        {
            if (board->hasPiece(newPos))
            {
                Piece *targetPiece = board->getPieceAt(newPos);
                if (targetPiece->getColor() != getColor())
                {
                    if (board->isSafeMove({currentPos, newPos, this}))
                        legalMoves.insert(newPos);
                }
            }
            else if (board->isSafeMove({currentPos, newPos, this}))
                legalMoves.insert(newPos);
        }
    }

    return legalMoves;
}