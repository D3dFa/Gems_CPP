// Square.h
#ifndef SQUARE_H
#define SQUARE_H

#include "Color.h"
#include "Bonus.h"

class Square {
private:
    Color color;
    Bonus* bonus;

public:
    Square(Color c = Color::EMPTY) : color(c), bonus(nullptr) {}
    ~Square() { delete bonus; }

    Color getColor() const { return color; }
    void setColor(Color c) { color = c; }

    Bonus* getBonus() const { return bonus; }
    void setBonus(Bonus* b) {
        delete bonus;
        bonus = b;
    }

    bool hasBonus() const { return bonus != nullptr; }
};

#endif // SQUARE_H
