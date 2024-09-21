// Board.cpp
#include "Board.h"
#include "Bonus.h"
#include <iostream>
#include <algorithm>
#include <queue>

// Конструктор
Board::Board(int w, int h) : width(w), height(h), grid(h, std::vector<Square>(w)), gen(std::random_device{}()) {
    initialize();
}

// Инициализация поля с случайными цветами, избегая начальных совпадений
void Board::initialize() {
    std::uniform_int_distribution<> dis(0, 5); // Предполагаем 6 цветов

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            Color newColor;
            bool conflict;
            do {
                conflict = false;
                newColor = static_cast<Color>(dis(gen));
                // Проверка горизонтальных совпадений
                if (j >= 2 && grid[i][j - 1].getColor() == newColor && grid[i][j - 2].getColor() == newColor) {
                    conflict = true;
                }
                // Проверка вертикальных совпадений
                if (i >= 2 && grid[i - 1][j].getColor() == newColor && grid[i - 2][j].getColor() == newColor) {
                    conflict = true;
                }
            } while (conflict);
            grid[i][j].setColor(newColor);
        }
    }
}

// Отображение игрового поля
void Board::display() const {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            switch (grid[i][j].getColor()) {
            case Color::RED: std::cout << "R "; break;
            case Color::GREEN: std::cout << "G "; break;
            case Color::BLUE: std::cout << "B "; break;
            case Color::YELLOW: std::cout << "Y "; break;
            case Color::PURPLE: std::cout << "P "; break;
            case Color::ORANGE: std::cout << "O "; break;
            default: std::cout << ". "; break;
            }
        }
        std::cout << "\n";
    }
}

// Обмен двух клеток
bool Board::swapSquares(int x1, int y1, int x2, int y2) {
    // Проверка, что квадраты соседние
    if ((abs(x1 - x2) + abs(y1 - y2)) != 1) {
        return false;
    }
    std::swap(grid[y1][x1], grid[y2][x2]);

    // Проверка на наличие совпадений после обмена
    auto hasMatch = [&](int x, int y) -> bool {
        Color current = grid[y][x].getColor();
        if (current == Color::EMPTY) return false;
        // Проверка горизонтали
        int count = 1;
        int j = x - 1;
        while (j >= 0 && grid[y][j].getColor() == current) { count++; j--; }
        j = x + 1;
        while (j < width && grid[y][j].getColor() == current) { count++; j++; }
        if (count >= 3) return true;
        // Проверка вертикали
        count = 1;
        int i = y - 1;
        while (i >= 0 && grid[i][x].getColor() == current) { count++; i--; }
        i = y + 1;
        while (i < height && grid[i][x].getColor() == current) { count++; i++; }
        return count >= 3;
        };

    bool match = hasMatch(x1, y1) || hasMatch(x2, y2);
    if (!match) {
        // Обмен не приводит к совпадениям, возвращаем назад
        std::swap(grid[y1][x1], grid[y2][x2]);
    }
    return match;
}

// Проверка и уничтожение совпадений
void Board::checkAndDestroyMatches() {
    // Используем BFS для поиска всех связанных одинаковых цветов >=3
    std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));
    bool anyMatch = false;
    std::vector<std::vector<std::pair<int, int>>> matches; // Хранение всех совпадений

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (!visited[i][j] && grid[i][j].getColor() != Color::EMPTY) {
                // BFS
                std::queue<std::pair<int, int>> q;
                std::vector<std::pair<int, int>> sameColor;
                q.push({ j, i });
                visited[i][j] = true;
                Color currentColor = grid[i][j].getColor();
                while (!q.empty()) {
                    auto [x, y] = q.front(); q.pop();
                    sameColor.emplace_back(x, y);
                    // Проверка соседей
                    std::vector<std::pair<int, int>> neighbors = { {x + 1,y}, {x - 1,y}, {x,y + 1}, {x,y - 1} };
                    for (auto& [nx, ny] : neighbors) {
                        if (nx >= 0 && nx < width && ny >= 0 && ny < height &&
                            !visited[ny][nx] && grid[ny][nx].getColor() == currentColor) {
                            visited[ny][nx] = true;
                            q.push({ nx, ny });
                        }
                    }
                }
                if (sameColor.size() >= 3) {
                    anyMatch = true;
                    matches.emplace_back(sameColor);
                }
            }
        }
    }

    // Уничтожение всех совпадений и выпадение бонусов
    for (const auto& group : matches) {
        for (const auto& [x, y] : group) {
            destroySquare(x, y);
        }
    }

    if (anyMatch) {
        dropSquares();
        checkAndDestroyMatches(); // Рекурсивная проверка после падения
    }
}

// Уничтожение клетки и возможное выпадение бонуса
void Board::destroySquare(int x, int y) {
    grid[y][x].setColor(Color::EMPTY);
    // Логика выпадения бонусов
    // С вероятностью, например, 10% выпадает бонус
    std::uniform_real_distribution<> dis(0.0, 1.0);
    double chance = dis(gen);
    if (chance <= 0.1) { // 10% шанс
        std::uniform_int_distribution<> bonusDis(0, 1);
        int bonusType = bonusDis(gen);
        if (bonusType == 0) {
            grid[y][x].setBonus(new RecolorBonus());
        }
        else {
            grid[y][x].setBonus(new BombBonus());
        }
    }
}

// Применение бонусов
void Board::applyBonuses() {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (grid[i][j].hasBonus()) {
                grid[i][j].getBonus()->apply(*this, j, i);
                grid[i][j].setBonus(nullptr); // Удаляем бонус после применения
            }
        }
    }
}

// Опускание клеток вниз и заполнение пустых мест новыми клетками
void Board::dropSquares() {
    for (int j = 0; j < width; ++j) {
        int emptySpot = height - 1;
        for (int i = height - 1; i >= 0; --i) {
            if (grid[i][j].getColor() != Color::EMPTY) {
                if (i != emptySpot) {
                    grid[emptySpot][j].setColor(grid[i][j].getColor());
                    grid[i][j].setColor(Color::EMPTY);
                }
                emptySpot--;
            }
        }
        // Заполнение пустых клеток новыми цветами
        std::uniform_int_distribution<> dis(0, 5);
        for (int i = emptySpot; i >= 0; --i) {
            grid[i][j].setColor(static_cast<Color>(dis(gen)));
        }
    }
}

// Получение цвета клетки
Color Board::getColor(int x, int y) const {
    return grid[y][x].getColor();
}

// Установка цвета клетки
void Board::setColor(int x, int y, Color c) {
    grid[y][x].setColor(c);
}

// Получение соседей в заданном радиусе
std::vector<std::pair<int, int>> Board::getNeighbors(int x, int y, int radius) const {
    std::vector<std::pair<int, int>> neighbors;
    for (int i = std::max(0, y - radius); i <= std::min(height - 1, y + radius); ++i) {
        for (int j = std::max(0, x - radius); j <= std::min(width - 1, x + radius); ++j) {
            if (!(i == y && j == x)) {
                neighbors.emplace_back(j, i);
            }
        }
    }
    return neighbors;
}

// Получение всех клеток на поле
std::vector<std::pair<int, int>> Board::getAllSquares() const {
    std::vector<std::pair<int, int>> all;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            all.emplace_back(j, i);
        }
    }
    return all;
}

// Проверка, являются ли две клетки несоседними
bool Board::areNonAdjacent(int x1, int y1, int x2, int y2) const {
    return (abs(x1 - x2) + abs(y1 - y2)) > 1;
}
