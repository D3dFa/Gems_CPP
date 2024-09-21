// Game.cpp
#include "Game.h"
#include <iostream>
#include <utility>
#include <limits>

void Game::run() {
    while (true) {
        board.display();
        std::cout << "������� ���������� ���� �������� ��������� ��� ������ (x1 y1 x2 y2), ��� -1 ��� ������:\n";
        int x1, y1, x2, y2;
        std::cin >> x1;
        if (x1 == -1) break;
        std::cin >> y1 >> x2 >> y2;
        if (board.swapSquares(x1, y1, x2, y2)) {
            board.checkAndDestroyMatches();
            board.applyBonuses();
        }
        else {
            std::cout << "�������� �����. ���������� �����.\n";
        }

        // ������� ��������� ������ ����� ��� ������
        if (std::cin.fail()) {
            std::cin.clear(); // ����� ������
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ������������� ������������ ��������
            std::cout << "������������ ����. ���������� �����.\n";
        }
    }
    std::cout << "���� ��������.\n";
}
