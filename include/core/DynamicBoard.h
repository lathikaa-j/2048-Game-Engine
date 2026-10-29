
#ifndef DYNAMIC_BOARD_H
#define DYNAMIC_BOARD_H

#include <vector>
#include <random>

class DynamicBoard {
private:
    int boardSize;
    std::vector<std::vector<int>> grid;
    std::mt19937 rng;

public:
    explicit DynamicBoard(int size = 4);

    int getSize() const;
    int getTile(int row, int col) const;
    void setTile(int row, int col, int value);

    void spawnRandomTile();
    bool moveLeft();
    bool moveRight();
    bool moveUp();
    bool moveDown();

    bool isFull() const;
    void reset();

private:
    bool moveLeftInRow(std::vector<int>& row);
    void validatePosition(int row, int col) const;
};

#endif
