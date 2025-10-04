#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "UI.h"
#include <unordered_set>

class Game
{
private:
    Board *board;
    UI *ui;
    Color currentPlayer = WHITE;
    Position selectedSquare = {-1, -1};
    unordered_set<Position, PositionHash> legalMoves;

public:
    Game();
    void run();
    void handleClick(int xCoord, int yCoord);
    bool hasSelected() const;
    ~Game();
};

#endif