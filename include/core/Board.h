
#pragma once

#include "core/Tile.h"

#include <cstddef>
#include <iosfwd>
#include <random>
#include <vector>

class Board {
private:
    std::size_t boardSize;
    std::vector<std::vector<Tile>> grid;
    std::mt19937 randomGenerator;
    int lastMoveScore;

    std::vector<int> processLine(const std::vector<int>& line);
    bool moveLines(bool horizontal, bool reversed);

public:
    static constexpr std::size_t MIN_BOARD_SIZE = 3;
    static constexpr std::size_t BOARD_SIZE = 4;
    static constexpr std::size_t MAX_BOARD_SIZE = 5;
    static constexpr int WINNING_VALUE = 2048;

    static bool isAllowedSize(std::size_t size);
    explicit Board(std::size_t size = BOARD_SIZE);

    std::size_t getSize() const;
    Tile getTile(std::size_t row, std::size_t col) const;
    void setTile(std::size_t row, std::size_t col, int value);
    void clear();

    bool moveLeft();
    bool moveRight();
    bool moveUp();
    bool moveDown();

    bool addRandomTile();
    bool addRandomObstacle();
    std::size_t getObstacleCount() const;
    int getLastMoveScore() const;
    bool hasWon() const;
    bool canMove() const;
    bool isGameOver() const;

    bool operator==(const Board& other) const;
    bool operator!=(const Board& other) const;

    int getHighestTile() const;
    friend std::ostream& operator<<(std::ostream& os, const Board& board);
};
