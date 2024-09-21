// Bonus.cpp
#include "Bonus.h"
#include "Board.h"
#include <vector>
#include <algorithm>
#include <random>
#include <utility>

// ���������� RecolorBonus
void RecolorBonus::apply(Board& board, int x, int y) {
    // �������� ������������ ���� ������
    Color originalColor = board.getColor(x, y);

    // ������������� ���� ������
    board.setColor(x, y, originalColor);

    // �������� ������� � ������� 3
    std::vector<std::pair<int, int>> neighbors = board.getNeighbors(x, y, 3);

    if (neighbors.size() < 2) return; // ���� ������������ �������, ������ �� ������

    // ������������ ������� ��� ���������� ������
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

// ���������� BombBonus
void BombBonus::apply(Board& board, int x, int y) {
    // �������� ��� ������ �� ����
    std::vector<std::pair<int, int>> allSquares = board.getAllSquares();

    // ������� (x, y) �� ������, ����� �������������, ��� ��� ����� ��������
    allSquares.erase(std::remove(allSquares.begin(), allSquares.end(), std::make_pair(x, y)), allSquares.end());

    // ������������ ���������� ������
    std::shuffle(allSquares.begin(), allSquares.end(), board.getGenerator());

    // �������� 4 ��������� ������
    if (allSquares.size() > 4) {
        allSquares.resize(4);
    }

    // ��������� (x, y) � ������ ������ ��� �����������
    allSquares.emplace_back(x, y);

    // ���������� ��������� ������
    for (const auto& [i, j] : allSquares) {
        board.destroySquare(i, j);
    }
}
