#include <iostream>
#include "../headers/UI.h"
#include <SDL2/SDL_image.h>
using namespace std;

UI::UI()
{
    initSDL();
    IMG_Init(IMG_INIT_PNG);
    boardWidth = 800, boardHeight = 800;
    createWindow();
    createRenderer();
    loadTextures();
}

void UI::initSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! " << SDL_GetError() << "\n";
        exit(1);
    }
}

void UI::createWindow()
{
    window = SDL_CreateWindow("Bishop's Bash",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              boardWidth, boardHeight,
                              SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "Window could not be created! " << SDL_GetError() << "\n";
        SDL_Quit();
        exit(1);
    }
}

void UI::createRenderer()
{
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "Renderer could not be created! " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }
}

int UI::getSquareSize()
{
    return boardWidth / 8.0;
}

void UI::drawChessSquares()
{
    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            SDL_Rect square = {col * getSquareSize(), row * getSquareSize(), getSquareSize(), getSquareSize()};
            if (((row + col) & 1) == 0)
                SDL_SetRenderDrawColor(renderer, 239, 239, 211, 255);
            else
                SDL_SetRenderDrawColor(renderer, 122, 153, 91, 255);
            SDL_RenderFillRect(renderer, &square);
        }
    }
}

void UI::loadTextures()
{
    for (int color = 0; color < 2; ++color)
    {
        for (int t = static_cast<int>(PieceType::PAWN); t <= static_cast<int>(PieceType::KING); ++t)
        {
            auto type = static_cast<PieceType>(t);

            string filename;
            filename += (color == static_cast<int>(Color::WHITE) ? "w" : "b");

            switch (type)
            {
            case PieceType::PAWN:
                filename += "p";
                break;
            case PieceType::ROOK:
                filename += "r";
                break;
            case PieceType::KNIGHT:
                filename += "n";
                break;
            case PieceType::BISHOP:
                filename += "b";
                break;
            case PieceType::QUEEN:
                filename += "q";
                break;
            case PieceType::KING:
                filename += "k";
                break;
            }

            filename = "assets/" + filename + ".png";

            SDL_Texture *tex = IMG_LoadTexture(renderer, filename.c_str());
            if (!tex)
                cout << "Failed to load " << filename << " : " << IMG_GetError() << '\n';

            textures[{static_cast<Color>(color), type}] = tex;
        }
    }
}

void UI::drawPiece(PieceType type, Color color, int x, int y, int size)
{
    SDL_Texture *texture = textures[{color, type}];

    if (texture)
    {
        SDL_Rect dest = {x, y, size, size};
        SDL_RenderCopy(renderer, texture, nullptr, &dest);
    }
}

void UI::drawPieces(const Board *board)
{
    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            Piece *piece = board->getPieceAt({row, col});
            if (piece)
                drawPiece(piece->getPieceType(), piece->getColor(), col * getSquareSize(), row * getSquareSize(), getSquareSize());
        }
    }
}

void UI::drawHighlightedSquares(Position selectedPosition, std::unordered_set<Position, PositionHash> legalMoves)
{
    if (selectedPosition.row != -1 && selectedPosition.col != -1)
    {
        SDL_Rect selectedRect = {selectedPosition.col * getSquareSize(), selectedPosition.row * getSquareSize(), getSquareSize(), getSquareSize()};
        SDL_SetRenderDrawColor(renderer, 186, 202, 68, 150);
        SDL_RenderFillRect(renderer, &selectedRect);
    }

    SDL_SetRenderDrawColor(renderer, 220, 220, 80, 150);
    for (const auto &pos : legalMoves)
    {
        SDL_Rect moveRect = {pos.col * getSquareSize(), pos.row * getSquareSize(), getSquareSize(), getSquareSize()};
        SDL_RenderFillRect(renderer, &moveRect);
    }
}

void UI::drawBoard(const Board *board, Position selectedPosition, std::unordered_set<Position, PositionHash> legalMoves)
{
    drawChessSquares();
    drawHighlightedSquares(selectedPosition, legalMoves);
    drawPieces(board);
}

void UI::render()
{
    SDL_RenderPresent(renderer);
}

UI::~UI()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}
