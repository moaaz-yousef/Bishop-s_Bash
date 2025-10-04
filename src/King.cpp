#include "../headers/King.h"
#include "../headers/Rook.h"

bool King::getHasMoved() const
{
    return hasMoved;
}

void King::markAsMoved()
{
    hasMoved = true;
}

std::unordered_set<Position, PositionHash> King::getLegalMoves(Position currentPos, Board *board)
{
    std::unordered_set<Position, PositionHash> legalMoves;
    for (int rowOffset = -1; rowOffset <= 1; ++rowOffset)
    {
        for (int colOffset = -1; colOffset <= 1; ++colOffset)
        {
            if (rowOffset == 0 && colOffset == 0)
                continue;

            Position newPos = {currentPos.row + rowOffset, currentPos.col + colOffset};
            if (newPos.row >= 0 && newPos.row < 8 && newPos.col >= 0 && newPos.col < 8)
            {
                if (board->hasPiece(newPos))
                {
                    Piece *targetPiece = board->getPieceAt(newPos);
                    if (targetPiece->getColor() != getColor() && board->isSafeMove({currentPos, newPos, this}))
                        legalMoves.insert(newPos);
                }
                else if (board->isSafeMove({currentPos, newPos, this}))
                    legalMoves.insert(newPos);
            }
        }
    }

    // Castling
    if (getColor() == Color::WHITE)
    {
        // Kingside
        if (board->hasPiece({7, 7}) && board->getPieceAt({7, 7})->getPieceType() == PieceType::ROOK && !((Rook *)board->getPieceAt({7, 7}))->getHasMoved() && !getHasMoved())
        {
            if (!board->hasPiece({7, 5}) && !board->hasPiece({7, 6}))
            {
                // check if squares are under attack (king's path)
                if (board->isSafeMove({currentPos, {7, 4}, this}) && board->isSafeMove({currentPos, {7, 5}, this}) && board->isSafeMove({currentPos, {7, 6}, this}))
                    legalMoves.insert({7, 6});
            }
        }

        // Queenside
        if (board->hasPiece({7, 0}) && board->getPieceAt({7, 0})->getPieceType() == PieceType::ROOK && !((Rook *)board->getPieceAt({7, 0}))->getHasMoved() && !getHasMoved())
        {
            if (!board->hasPiece({7, 1}) && !board->hasPiece({7, 2}) && !board->hasPiece({7, 3}))
            {
                // check if squares are under attack (king's path)
                if (board->isSafeMove({currentPos, {7, 2}, this}) && board->isSafeMove({currentPos, {7, 3}, this}) && board->isSafeMove({currentPos, {7, 4}, this}))
                    legalMoves.insert({7, 2});
            }
        }
    }
    else
    {
        // Kingside
        if (board->hasPiece({0, 7}) && board->getPieceAt({0, 7})->getPieceType() == PieceType::ROOK && !((Rook *)board->getPieceAt({0, 7}))->getHasMoved() && !getHasMoved())
        {
            if (!board->hasPiece({0, 5}) && !board->hasPiece({0, 6}))
            {
                // check if squares are under attack (king's path)
                if (board->isSafeMove({currentPos, {0, 4}, this}) && board->isSafeMove({currentPos, {0, 5}, this}) && board->isSafeMove({currentPos, {0, 6}, this}))
                    legalMoves.insert({0, 6});
            }
        }

        // Queenside
        if (board->hasPiece({0, 0}) && board->getPieceAt({0, 0})->getPieceType() == PieceType::ROOK && !((Rook *)board->getPieceAt({0, 0}))->getHasMoved() && !getHasMoved())
        {
            if (!board->hasPiece({0, 1}) && !board->hasPiece({0, 2}) && !board->hasPiece({0, 3}))
            {
                // check if squares are under attack (king's path)
                if (board->isSafeMove({currentPos, {0, 2}, this}) && board->isSafeMove({currentPos, {0, 3}, this}) && board->isSafeMove({currentPos, {0, 4}, this}))
                    legalMoves.insert({0, 2});
            }
        }
    }

    return legalMoves;
}
