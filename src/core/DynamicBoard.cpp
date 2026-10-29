
#include "core/DynamicBoard.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

DynamicBoard::DynamicBoard(int size)
    : boardSize(size),
      grid(size > 0 ? size : 0,
           std::vector<int>(size > 0 ? size : 0, 0)),
      rng(std::random_device{}()) {
    if (size <= 0) {
        throw std::invalid_argument("Board size must be positive");
    }

    spawnRandomTile();
    spawnRandomTile();
}

int DynamicBoard::getSize() const {
    return boardSize;
}

void DynamicBoard::validatePosition(int row, int col) const {
    if (row < 0 || row >= boardSize ||
        col < 0 || col >= boardSize) {
        throw std::out_of_range("Board position is out of range");
    }
}

int DynamicBoard::getTile(int row, int col) const {
    validatePosition(row, col);
    return grid[row][col];
}

void DynamicBoard::setTile(int row, int col, int value) {
    validatePosition(row, col);

    if (value < 0) {
        throw std::invalid_argument("Tile value cannot be negative");
    }

    grid[row][col] = value;
}

void DynamicBoard::spawnRandomTile() {
    std::vector<std::pair<int, int>> emptyCells;

    for (int r = 0; r < boardSize; ++r) {
        for (int c = 0; c < boardSize; ++c) {
            if (grid[r][c] == 0) {
                emptyCells.emplace_back(r, c);
            }
        }
    }

    if (emptyCells.empty()) {
        return;
    }

    std::uniform_int_distribution<int> cellDist(
        0, static_cast<int>(emptyCells.size()) - 1
    );

    std::uniform_int_distribution<int> valueDist(1, 10);

    const auto [row, col] = emptyCells[cellDist(rng)];

    // 90% chance of spawning 2; 10% chance of spawning 4.
    grid[row][col] = (valueDist(rng) == 10) ? 4 : 2;
}

bool DynamicBoard::moveLeftInRow(std::vector<int>& row) {
    std::vector<int> original = row;
    std::vector<int> values;

    // Remove empty cells.
    for (int value : row) {
        if (value != 0) {
            values.push_back(value);
        }
    }

    std::vector<int> result;

    // Merge adjacent equal tiles once per move.
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (i + 1 < values.size() &&
            values[i] == values[i + 1]) {
            result.push_back(values[i] * 2);
            ++i;
        } else {
            result.push_back(values[i]);
        }
    }

    result.resize(boardSize, 0);
    row = result;

    return row != original;
}

bool DynamicBoard::moveLeft() {
    bool changed = false;

    for (auto& row : grid) {
        if (moveLeftInRow(row)) {
            changed = true;
        }
    }

    if (changed) {
        spawnRandomTile();
    }

    return changed;
}

bool DynamicBoard::moveRight() {
    bool changed = false;

    for (auto& row : grid) {
        std::reverse(row.begin(), row.end());

        if (moveLeftInRow(row)) {
            changed = true;
        }

        std::reverse(row.begin(), row.end());
    }

    if (changed) {
        spawnRandomTile();
    }

    return changed;
}

bool DynamicBoard::moveUp() {
    // Transpose the board so columns become rows.
    std::vector<std::vector<int>> transposed(
        boardSize, std::vector<int>(boardSize, 0)
    );

    for (int r = 0; r < boardSize; ++r) {
        for (int c = 0; c < boardSize; ++c) {
            transposed[c][r] = grid[r][c];
        }
    }

    bool changed = false;

    for (auto& row : transposed) {
        if (moveLeftInRow(row)) {
            changed = true;
        }
    }

    if (changed) {
        for (int r = 0; r < boardSize; ++r) {
            for (int c = 0; c < boardSize; ++c) {
                grid[c][r] = transposed[r][c];
            }
        }

        spawnRandomTile();
    }

    return changed;
}

bool DynamicBoard::moveDown() {
    // Transpose the board and reverse each row to move downward.
    std::vector<std::vector<int>> transposed(
        boardSize, std::vector<int>(boardSize, 0)
    );

    for (int r = 0; r < boardSize; ++r) {
        for (int c = 0; c < boardSize; ++c) {
            transposed[c][r] = grid[r][c];
        }
    }

    bool changed = false;

    for (auto& row : transposed) {
        std::reverse(row.begin(), row.end());

        if (moveLeftInRow(row)) {
            changed = true;
        }

        std::reverse(row.begin(), row.end());
    }

    if (changed) {
        for (int r = 0; r < boardSize; ++r) {
            for (int c = 0; c < boardSize; ++c) {
                grid[c][r] = transposed[r][c];
            }
        }

        spawnRandomTile();
    }

    return changed;
}

bool DynamicBoard::isFull() const {
    for (const auto& row : grid) {
        for (int value : row) {
            if (value == 0) {
                return false;
            }
        }
    }

    return true;
}

void DynamicBoard::reset() {
    for (auto& row : grid) {
        std::fill(row.begin(), row.end(), 0);
    }

    spawnRandomTile();
    spawnRandomTile();
}
