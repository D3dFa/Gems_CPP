// Game.cpp
#include "Game.h"
#include <iostream>
#include <utility>
#include <limits>

void Game::run() {
    while (true) {
        board.display();
        std::cout << "Введите координаты двух соседних квадратов для обмена (x1 y1 x2 y2), или -1 для выхода:\n";
        int x1, y1, x2, y2;
        std::cin >> x1;
        if (x1 == -1) break;
        std::cin >> y1 >> x2 >> y2;
        if (board.swapSquares(x1, y1, x2, y2)) {
            board.checkAndDestroyMatches();
            board.applyBonuses();
        }
        else {
            std::cout << "Неверный обмен. Попробуйте снова.\n";
        }

        // Очистка состояния потока ввода при ошибке
        if (std::cin.fail()) {
            std::cin.clear(); // Сброс ошибок
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Игнорирование некорректных символов
            std::cout << "Некорректный ввод. Попробуйте снова.\n";
        }
    }
    std::cout << "Игра окончена.\n";
}
