// Game.h
#ifndef GAME_H
#define GAME_H

#include "Board.h"

class Game {
private:
    Board board;

public:
    Game(int width = 8, int height = 8) : board(width, height) {}
    void run();
};

#endif // GAME_H
