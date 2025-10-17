#ifndef BOARD_H
#define BOARD_H

#include <map>
#include "Piece.h"
#include "Game.h"
#include "../headers/Types.h"

using namespace std;

class Game;

class Board
{
private:
    Game *game;
    Piece *cells[8][8];
    Piece *scanDirection(Position start, int xDirection, int yDirection);
    Move lastMove = {{-1, -1}, {-1, -1}, nullptr};
    int seventyFiveMoveCounter = 0;

public:
    Board(Game *game);
    bool hasPiece(Position position) const;
    Piece *getPieceAt(Position position) const;
    void movePiece(Position selectedSquare, Position position);
    bool isKingInCheck(Color color);
    bool isThereAvailableMove(Color color);
    bool isSafeMove(Move move);
    Move getLastMove() const;
    void setLastMove(Move move);
    bool isInsufficientMaterial();
    bool isSeventyFiveMoveRule();
    ~Board();
};

#endif