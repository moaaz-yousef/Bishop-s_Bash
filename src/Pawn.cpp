#include <math.h>
#include "../headers/Pawn.h"

std::unordered_set<Position, PositionHash> Pawn::getLegalMoves(Position currentPos, Board *board)
{
    std::unordered_set<Position, PositionHash> legalMoves;
    int direction = (getColor() == Color::WHITE) ? -1 : 1;

    // Forward move
    if (board->hasPiece({currentPos.row + direction, currentPos.col}) == false)
    {
        if (board->isSafeMove({currentPos, {currentPos.row + direction, currentPos.col}, this}))
            legalMoves.insert({currentPos.row + direction, currentPos.col});

        if ((getColor() == Color::WHITE && currentPos.row == 6) || (getColor() == Color::BLACK && currentPos.row == 1))
        {
            if (board->hasPiece({currentPos.row + 2 * direction, currentPos.col}) == false && board->isSafeMove({currentPos, {currentPos.row + 2 * direction, currentPos.col}, this}))
            {
                legalMoves.insert({currentPos.row + 2 * direction, currentPos.col});
            }
        }
    }

    // Capture moves
    for (int colOffset : {-1, 1})
    {
        Position attackPos = {currentPos.row + direction, currentPos.col + colOffset};
        if (board->hasPiece(attackPos))
        {
            Piece *targetPiece = board->getPieceAt(attackPos);
            if (targetPiece->getColor() != getColor() & board->isSafeMove({currentPos, attackPos, this}))
                legalMoves.insert(attackPos);
        }
    }

    // En passant
    Move lastMove = board->getLastMove();
    if (lastMove.movedPiece != nullptr && lastMove.movedPiece->getPieceType() == PieceType::PAWN && lastMove.movedPiece->getColor() != getColor())
    {
        if (abs(lastMove.from.row - lastMove.to.row) == 2 && lastMove.to.row == currentPos.row)
        {
            if (abs(lastMove.to.col - currentPos.col) == 1)
            {
                Position enPassantPos = {currentPos.row + direction, lastMove.to.col};
                if (board->isSafeMove({currentPos, enPassantPos, this}))
                    legalMoves.insert(enPassantPos);
            }
        }
    }

    return legalMoves;
}