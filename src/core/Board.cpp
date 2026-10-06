#include "core/Board.h"

Board::Board()
    : grid(BOARD_SIZE, std::vector<Tile>(BOARD_SIZE)),
      randomGenerator(std::random_device{}()), lastMoveScore(0) {}

std::size_t Board::getSize() const {
    return BOARD_SIZE;
}

Tile Board::getTile(std::size_t row, std::size_t col) const {
    return grid.at(row).at(col);
}

void Board::setTile(std::size_t row, std::size_t col, int value) {
    grid.at(row).at(col).setValue(value);
}

void Board::clear() {
    lastMoveScore = 0;
    for (auto& row : grid) {
        for (Tile& tile : row) {
            tile.setValue(0);
        }
    }
}

std::vector<int> Board::processLine(const std::vector<int>& line) {
    std::vector<int> nonEmpty;
    for (int value : line) {
        if (value != 0) {
            nonEmpty.push_back(value);
        }
    }

    std::vector<int> result;
    for (std::size_t index = 0; index < nonEmpty.size(); ++index) {
        if (index + 1 < nonEmpty.size() && nonEmpty[index] == nonEmpty[index + 1]) {
            const int mergedValue = nonEmpty[index] * 2;
            result.push_back(mergedValue);
            lastMoveScore += mergedValue;
            ++index; // Consume both original tiles; the result cannot merge again.
        } else {
            result.push_back(nonEmpty[index]);
        }
    }
    result.resize(BOARD_SIZE, 0);
    return result;
}

bool Board::moveLines(bool horizontal, bool reversed) {
    lastMoveScore = 0;
    bool changed = false;
    for (std::size_t lineIndex = 0; lineIndex < BOARD_SIZE; ++lineIndex) {
        std::vector<int> line;
        for (std::size_t offset = 0; offset < BOARD_SIZE; ++offset) {
            // Read starting at the destination edge of the move.
            const std::size_t position = reversed ? BOARD_SIZE - 1 - offset : offset;
            const std::size_t row = horizontal ? lineIndex : position;
            const std::size_t col = horizontal ? position : lineIndex;
            line.push_back(grid[row][col].getValue());
        }

        const std::vector<int> result = processLine(line);
        if (result != line) {
            changed = true;
            for (std::size_t offset = 0; offset < BOARD_SIZE; ++offset) {
                const std::size_t position = reversed ? BOARD_SIZE - 1 - offset : offset;
                const std::size_t row = horizontal ? lineIndex : position;
                const std::size_t col = horizontal ? position : lineIndex;
                grid[row][col].setValue(result[offset]);
            }
        }
    }
    return changed;
}

bool Board::moveLeft() {
    return moveLines(true, false);
}

bool Board::moveRight() {
    return moveLines(true, true);
}

bool Board::moveUp() {
    return moveLines(false, false);
}

bool Board::moveDown() {
    return moveLines(false, true);
}

bool Board::addRandomTile() {
    // Store flattened positions; the grid remains private.
    std::vector<std::size_t> emptyPositions;
    for (std::size_t row = 0; row < BOARD_SIZE; ++row) {
        for (std::size_t col = 0; col < BOARD_SIZE; ++col) {
            if (grid[row][col].isEmpty()) {
                emptyPositions.push_back(row * BOARD_SIZE + col);
            }
        }
    }
    if (emptyPositions.empty()) {
        return false;
    }

    std::uniform_int_distribution<std::size_t> positionDistribution(0, emptyPositions.size() - 1);
    std::uniform_int_distribution<int> valueDistribution(1, 10);
    const std::size_t position = emptyPositions[positionDistribution(randomGenerator)];
    const int value = valueDistribution(randomGenerator) <= 9 ? 2 : 4;
    grid[position / BOARD_SIZE][position % BOARD_SIZE].setValue(value);
    return true;
}

int Board::getLastMoveScore() const {
    return lastMoveScore;
}

bool Board::hasWon() const {
    for (const auto& row : grid) {
        for (const Tile& tile : row) {
            if (tile.getValue() >= WINNING_VALUE) {
                return true;
            }
        }
    }
    return false;
}

bool Board::canMove() const {
    for (std::size_t row = 0; row < BOARD_SIZE; ++row) {
        for (std::size_t col = 0; col < BOARD_SIZE; ++col) {
            const int value = grid[row][col].getValue();
            if (grid[row][col].isEmpty()
                || (col + 1 < BOARD_SIZE && value == grid[row][col + 1].getValue())
                || (row + 1 < BOARD_SIZE && value == grid[row + 1][col].getValue())) {
                return true;
            }
        }
    }
    return false;
}

bool Board::isGameOver() const {
    return !canMove();
}
