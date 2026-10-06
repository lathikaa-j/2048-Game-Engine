#include "core/Board.h"

Board::Board() : grid(BOARD_SIZE, std::vector<Tile>(BOARD_SIZE)) {}

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
    for (auto& row : grid) {
        for (Tile& tile : row) {
            tile.setValue(0);
        }
    }
}

std::vector<int> Board::processLine(const std::vector<int>& line) const {
    std::vector<int> nonEmpty;
    for (int value : line) {
        if (value != 0) {
            nonEmpty.push_back(value);
        }
    }

    std::vector<int> result;
    for (std::size_t index = 0; index < nonEmpty.size(); ++index) {
        if (index + 1 < nonEmpty.size() && nonEmpty[index] == nonEmpty[index + 1]) {
            result.push_back(nonEmpty[index] * 2);
            ++index; // Consume both original tiles; the result cannot merge again.
        } else {
            result.push_back(nonEmpty[index]);
        }
    }
    result.resize(BOARD_SIZE, 0);
    return result;
}

bool Board::moveLines(bool horizontal, bool reversed) {
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
