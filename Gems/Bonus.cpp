// Bonus.cpp
#include "Bonus.h"
#include "Board.h"
#include <vector>
#include <algorithm>
#include <random>
#include <utility>

// Реализация RecolorBonus
void RecolorBonus::apply(Board& board, int x, int y) {
    // Получаем оригинальный цвет клетки
    Color originalColor = board.getColor(x, y);

    // Перекрашиваем саму клетку
    board.setColor(x, y, originalColor);

    // Получаем соседей в радиусе 3
    std::vector<std::pair<int, int>> neighbors = board.getNeighbors(x, y, 3);

    if (neighbors.size() < 2) return; // Если недостаточно соседей, ничего не делаем

    // Перемешиваем соседей для случайного выбора
    std::shuffle(neighbors.begin(), neighbors.end(), board.getGenerator());

    int recolored = 0;
    for (const auto& [nx, ny] : neighbors) {
        if (board.areNonAdjacent(x, y, nx, ny)) {
            board.setColor(nx, ny, originalColor);
            recolored++;
            if (recolored >= 2) break;
        }
    }
}

// Реализация BombBonus
void BombBonus::apply(Board& board, int x, int y) {
    // Получаем все клетки на поле
    std::vector<std::pair<int, int>> allSquares = board.getAllSquares();

    // Удаляем (x, y) из списка, чтобы гарантировать, что она будет включена
    allSquares.erase(std::remove(allSquares.begin(), allSquares.end(), std::make_pair(x, y)), allSquares.end());

    // Перемешиваем оставшиеся клетки
    std::shuffle(allSquares.begin(), allSquares.end(), board.getGenerator());

    // Выбираем 4 случайных клетки
    if (allSquares.size() > 4) {
        allSquares.resize(4);
    }

    // Добавляем (x, y) к списку клеток для уничтожения
    allSquares.emplace_back(x, y);

    // Уничтожаем выбранные клетки
    for (const auto& [i, j] : allSquares) {
        board.destroySquare(i, j);
    }
}
