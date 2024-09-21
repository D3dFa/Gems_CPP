// Bonus.h
#ifndef BONUS_H
#define BONUS_H

// Forward declaration класса Board
class Board;

class Bonus {
public:
    virtual void apply(Board& board, int x, int y) = 0;
    virtual ~Bonus() {}
};

class RecolorBonus : public Bonus {
public:
    void apply(Board& board, int x, int y) override;
};

class BombBonus : public Bonus {
public:
    void apply(Board& board, int x, int y) override;
};

#endif // BONUS_H
