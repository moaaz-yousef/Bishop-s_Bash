#include <iostream>
#include "../headers/Game.h"

using namespace std;

Game::Game()
{
    board = new Board(this);
    ui = new UI();
    ui->drawBoard(board, selectedSquare, legalMoves);
    ui->render();
}

void Game::run()
{
    bool running = true;
    SDL_Event e;

    while (running)
    {
        while (SDL_WaitEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                running = false;
                break;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);
                handleClick(x, y);
                ui->drawBoard(board, selectedSquare, legalMoves);
                ui->render();
            }
        }
    }
}

bool Game::hasSelected() const
{
    return selectedSquare.row != -1 && selectedSquare.col != -1;
}

void Game::handleClick(int xCoord, int yCoord)
{
    Position position = {yCoord / ui->getSquareSize(), xCoord / ui->getSquareSize()};

    if (!hasSelected())
    {
        if (board->hasPiece(position) && board->getPieceAt(position)->getColor() == currentPlayer)
        {
            selectedSquare = position;
            legalMoves = board->getPieceAt(position)->getLegalMoves(position, board);
        }
    }
    else
    {
        if (legalMoves.count(position))
        {
            board->movePiece(selectedSquare, position);
            currentPlayer = (currentPlayer == WHITE ? BLACK : WHITE);
            selectedSquare = {-1, -1};
            legalMoves.clear();

            // Check for checkmate or stalemate
            if (!board->isThereAvailableMove(currentPlayer))
            {
                if (board->isKingInCheck(currentPlayer))
                {
                    cout << (currentPlayer == WHITE ? "Black" : "White") << " wins by checkmate!" << endl;
                }
                else
                {
                    cout << "Stalemate! It's a draw." << endl;
                }
            }
        }
        else if (board->hasPiece(position) && board->getPieceAt(position)->getColor() == currentPlayer)
        {
            // reselect
            selectedSquare = position;
            legalMoves = board->getPieceAt(position)->getLegalMoves(position, board);
        }
        else
        {
            selectedSquare = {-1, -1};
            legalMoves.clear();
        }
    }
}

PieceType Game::handlePromotionRequest(Position position)
{
    ui->showPromotionOptions(position);
    SDL_Event e;
    while (true)
    {
        while (SDL_WaitEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                exit(0);
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);
                Position clickedPosition = {y / ui->getSquareSize(), x / ui->getSquareSize()};
                if (clickedPosition != position)
                    continue;

                int option = 0;
                int Xcenter = position.col * ui->getSquareSize() + ui->getSquareSize() / 2;
                int Ycenter = position.row * ui->getSquareSize() + ui->getSquareSize() / 2;
                if (x > Xcenter)
                    option += 1;
                if (y > Ycenter)
                    option += 2;

                switch (option)
                {
                case 0:
                    return PieceType::QUEEN;
                case 1:
                    return PieceType::ROOK;
                case 2:
                    return PieceType::BISHOP;
                case 3:
                    return PieceType::KNIGHT;
                }
            }
        }
    }
}

Game::~Game()
{
    delete board;
    delete ui;
}