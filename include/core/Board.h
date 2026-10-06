#pragma once

#include "core/Tile.h"

#include <cstddef>
#include <vector>

class Board {
private:
    std::vector<std::vector<Tile>> grid;

    std::vector<int> processLine(const std::vector<int>& line) const;
    bool moveLines(bool horizontal, bool reversed);

public:
    static constexpr std::size_t BOARD_SIZE = 4;

    Board();

    std::size_t getSize() const;
    // Returns a copy. Invalid indices use std::vector::at's bounds checking.
    Tile getTile(std::size_t row, std::size_t col) const;
    void setTile(std::size_t row, std::size_t col, int value);
    void clear();

    bool moveLeft();
    bool moveRight();
    bool moveUp();
    bool moveDown();
};
