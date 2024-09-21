// Board.h
#ifndef BOARD_H
#define BOARD_H

#include "Square.h"
#include <vector>
#include <utility>
#include <random>

class Board {
private:
    int width;
    int height;
    std::vector<std::vector<Square>> grid;
    std::mt19937 gen; // Генератор случайных чисел

public:
    Board(int w = 8, int h = 8);
    void initialize();
    void display() const;
    bool swapSquares(int x1, int y1, int x2, int y2);
    void checkAndDestroyMatches();
    void applyBonuses();
    void dropSquares();

    Color getColor(int x, int y) const;
    void setColor(int x, int y, Color c);
    void destroySquare(int x, int y);

    std::vector<std::pair<int, int>> getNeighbors(int x, int y, int radius) const;
    std::vector<std::pair<int, int>> getAllSquares() const;
    bool areNonAdjacent(int x1, int y1, int x2, int y2) const;

    // Метод для получения ссылки на генератор случайных чисел
    std::mt19937& getGenerator() { return gen; }
};

#endif // BOARD_H
