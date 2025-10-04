#ifndef UI_H
#define UI_H

#include <map>
#include <SDL2/SDL.h>
#include "Types.h"
#include "Board.h"

using namespace std;

class UI
{
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    map<pair<Color, PieceType>, SDL_Texture *> textures;
    int boardWidth, boardHeight;
    void initSDL();
    void createWindow();
    void createRenderer();
    void drawChessSquares();
    void drawHighlightedSquares(Position selectedPosition, std::unordered_set<Position, PositionHash> legalMoves);
    void drawPiece(PieceType type, Color color, int x, int y, int size);
    void drawPieces(const Board *board);
    void loadTextures();
    void showPromotionOptions();

public:
    UI();
    int getSquareSize();
    void drawBoard(const Board *board, Position selectedPosition, std::unordered_set<Position, PositionHash> legalMoves);
    void render();
    ~UI();
};

#endif