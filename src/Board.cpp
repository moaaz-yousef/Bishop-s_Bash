#include <iostream>
#include <map>
#include <SDL2/SDL_image.h>
#include "../headers/Piece.h"
#include "../headers/Bishop.h"
#include "../headers/King.h"
#include "../headers/Knight.h"
#include "../headers/Pawn.h"
#include "../headers/Queen.h"
#include "../headers/Rook.h"
#include "../headers/Board.h"

using namespace std;

Board::Board()
{
    // initialize pieces
    cells[0][0] = new Rook(Color::BLACK);
    cells[0][1] = new Knight(Color::BLACK);
    cells[0][2] = new Bishop(Color::BLACK);
    cells[0][3] = new Queen(Color::BLACK);
    cells[0][4] = new King(Color::BLACK);
    cells[0][5] = new Bishop(Color::BLACK);
    cells[0][6] = new Knight(Color::BLACK);
    cells[0][7] = new Rook(Color::BLACK);

    for (int col = 0; col < 8; ++col)
        cells[1][col] = new Pawn(Color::BLACK);

    for (int row = 2; row <= 5; ++row)
        for (int col = 0; col < 8; ++col)
            cells[row][col] = nullptr;

    for (int col = 0; col < 8; ++col)
        cells[6][col] = new Pawn(Color::WHITE);

    cells[7][0] = new Rook(Color::WHITE);
    cells[7][1] = new Knight(Color::WHITE);
    cells[7][2] = new Bishop(Color::WHITE);
    cells[7][3] = new Queen(Color::WHITE);
    cells[7][4] = new King(Color::WHITE);
    cells[7][5] = new Bishop(Color::WHITE);
    cells[7][6] = new Knight(Color::WHITE);
    cells[7][7] = new Rook(Color::WHITE);
}

bool Board::hasPiece(Position position) const
{
    if (position.row < 0 || position.row >= 8 || position.col < 0 || position.col >= 8)
        return false;
    return cells[position.row][position.col] != nullptr;
}

Piece *Board::getPieceAt(Position position) const
{
    if (position.row < 0 || position.row >= 8 || position.col < 0 || position.col >= 8)
        return nullptr;
    return cells[position.row][position.col];
}

void Board::movePiece(Position selectedSquare, Position position)
{
    if (selectedSquare.row < 0 || selectedSquare.row >= 8 || selectedSquare.col < 0 || selectedSquare.col >= 8 ||
        position.row < 0 || position.row >= 8 || position.col < 0 || position.col >= 8)
        return;

    Piece *movingPiece = cells[selectedSquare.row][selectedSquare.col];
    if (!movingPiece)
        return;

    // if the move is en passant, remove the captured pawn
    if (movingPiece->getPieceType() == PieceType::PAWN)
    {
        Pawn *pawn = (Pawn *)movingPiece;
        int direction = (pawn->getColor() == Color::WHITE) ? -1 : 1;
        if (selectedSquare.col != position.col && !hasPiece(position))
        {
            Position capturedPos = {position.row - direction, position.col};
            if (hasPiece(capturedPos) && getPieceAt(capturedPos)->getPieceType() == PieceType::PAWN && getPieceAt(capturedPos)->getColor() != pawn->getColor())
            {
                delete cells[capturedPos.row][capturedPos.col];
                cells[capturedPos.row][capturedPos.col] = nullptr;
            }
        }
    }

    if (cells[position.row][position.col] != nullptr)
    {
        delete cells[position.row][position.col];
        cells[position.row][position.col] = nullptr;
    }

    cells[position.row][position.col] = movingPiece;
    cells[selectedSquare.row][selectedSquare.col] = nullptr;

    // if the move is castling, move the rook
    if (movingPiece->getPieceType() == PieceType::KING)
    {
        King *king = (King *)movingPiece;
        king->markAsMoved();

        // Kingside
        if (selectedSquare.col == 4 && position.col == 6)
        {
            Piece *rookPiece = cells[selectedSquare.row][7];
            if (rookPiece && rookPiece->getPieceType() == PieceType::ROOK)
            {
                Rook *rook = (Rook *)rookPiece;
                rook->markAsMoved();
                cells[selectedSquare.row][5] = rookPiece;
                cells[selectedSquare.row][7] = nullptr;
            }
        }
        // Queenside
        else if (selectedSquare.col == 4 && position.col == 2)
        {
            Piece *rookPiece = cells[selectedSquare.row][0];
            if (rookPiece && rookPiece->getPieceType() == PieceType::ROOK)
            {
                Rook *rook = (Rook *)rookPiece;
                rook->markAsMoved();
                cells[selectedSquare.row][3] = rookPiece;
                cells[selectedSquare.row][0] = nullptr;
            }
        }
    }
    else if (movingPiece->getPieceType() == PieceType::ROOK)
    {
        Rook *rook = (Rook *)movingPiece;
        rook->markAsMoved();
    }

    setLastMove({selectedSquare, position, movingPiece});
}

Piece *Board::scanDirection(Position start, int xDirection, int yDirection)
{
    int x = start.row + xDirection;
    int y = start.col + yDirection;

    while (x >= 0 && x < 8 && y >= 0 && y < 8)
    {
        if (cells[x][y] != nullptr)
            return cells[x][y];
        x += xDirection;
        y += yDirection;
    }

    return nullptr;
}

bool Board::isKingInCheck(Color color)
{
    // Locate the king on the board
    Position kingPos = {-1, -1};
    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            Piece *piece = cells[row][col];
            if (piece != nullptr && piece->getColor() == color && piece->getPieceType() == PieceType::KING)
            {
                kingPos = {row, col};
                break;
            }
        }

        if (kingPos.row != -1)
            break;
    }

    if (kingPos.row == -1)
        return false; // King not found, should not happen in a valid game

    // Check for threats from all directions
    int directions[8][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
    for (auto &dir : directions)
    {
        Piece *threat = scanDirection(kingPos, dir[0], dir[1]);
        if (threat != nullptr && threat->getColor() != color)
        {
            PieceType type = threat->getPieceType();
            if ((dir[0] == 0 || dir[1] == 0) && (type == PieceType::ROOK || type == PieceType::QUEEN))
                return true;
            if ((dir[0] != 0 && dir[1] != 0) && (type == PieceType::BISHOP || type == PieceType::QUEEN))
                return true;
        }
    }

    // Check for knight threats
    int knightMoves[8][2] = {{-2, -1}, {-2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2}, {2, -1}, {2, 1}};
    for (auto &move : knightMoves)
    {
        int newRow = kingPos.row + move[0];
        int newCol = kingPos.col + move[1];
        if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8)
        {
            Piece *piece = cells[newRow][newCol];
            if (piece != nullptr && piece->getColor() != color && piece->getPieceType() == PieceType::KNIGHT)
                return true;
        }
    }

    // Check for pawn threats
    int pawnRow = (color == Color::WHITE) ? -1 : 1;
    Position threatPositions[2] = {{kingPos.row + pawnRow, kingPos.col - 1}, {kingPos.row + pawnRow, kingPos.col + 1}};
    for (int i = 0; i < 2; ++i)
    {
        Position pos = threatPositions[i];
        if (pos.row >= 0 && pos.row < 8 && pos.col >= 0 && pos.col < 8)
        {
            Piece *piece = cells[pos.row][pos.col];
            if (piece != nullptr && piece->getColor() != color && piece->getPieceType() == PieceType::PAWN)
                return true;
        }
    }

    // Check for king threat (if two kings are adjacent)
    for (int rowOffset = -1; rowOffset <= 1; ++rowOffset)
    {
        for (int colOffset = -1; colOffset <= 1; ++colOffset)
        {
            if (rowOffset == 0 && colOffset == 0)
                continue;

            Position position = {kingPos.row + rowOffset, kingPos.col + colOffset};
            if (position.row >= 0 && position.row < 8 && position.col >= 0 && position.col < 8)
            {
                Piece *piece = cells[position.row][position.col];
                if (piece != nullptr && piece->getColor() != color && piece->getPieceType() == PieceType::KING)
                    return true;
            }
        }
    }

    return false;
}

bool Board::isThereAvailableMove(Color color)
{
    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            Piece *piece = cells[row][col];
            if (piece != nullptr && piece->getColor() == color)
            {
                Position currentPos = {row, col};
                auto legalMoves = piece->getLegalMoves(currentPos, this);
                if (!legalMoves.empty())
                    return true;
            }
        }
    }

    return false;
}

bool Board::isSafeMove(Move move)
{
    // simulate move
    Piece *captured = cells[move.to.row][move.to.col];
    Piece *moving = cells[move.from.row][move.from.col];

    cells[move.from.row][move.from.col] = nullptr;
    cells[move.to.row][move.to.col] = moving;

    bool safe = !isKingInCheck(move.movedPiece->getColor());

    // undo move
    cells[move.from.row][move.from.col] = moving;
    cells[move.to.row][move.to.col] = captured;

    return safe;
}

Board::~Board()
{
    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            if (cells[row][col] != nullptr)
            {
                delete cells[row][col];
                cells[row][col] = nullptr;
            }
        }
    }
}

Move Board::getLastMove() const
{
    return lastMove;
}

void Board::setLastMove(Move move)
{
    lastMove = move;
}