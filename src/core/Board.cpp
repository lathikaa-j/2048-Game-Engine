#include "core/Board.h"
#include<stdexcept>
#include <iomanip>
#include <ostream>

bool Board::isAllowedSize(std::size_t size) {
    return size >= MIN_BOARD_SIZE && size <= MAX_BOARD_SIZE;
}

Board::Board(std::size_t size)
    : boardSize(size),
      grid(size, std::vector<Tile>(size)),
      randomGenerator(std::random_device{}()),
      lastMoveScore(0) {
    if (!isAllowedSize(size)) {
        throw std::invalid_argument("Board size must be 3, 4, or 5");
    }
}

std::size_t Board::getSize() const {
    return boardSize;
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
    result.resize(boardSize, 0);
    return result;
}

bool Board::moveLines(bool horizontal, bool reversed) {
    lastMoveScore = 0;
    bool changed = false;
    for (std::size_t lineIndex = 0; lineIndex < boardSize; ++lineIndex) {
        std::vector<int> line;
        for (std::size_t offset = 0; offset < boardSize; ++offset) {
            // Read starting at the destination edge of the move.
            const std::size_t position = reversed ? boardSize - 1 - offset : offset;
            const std::size_t row = horizontal ? lineIndex : position;
            const std::size_t col = horizontal ? position : lineIndex;
            line.push_back(grid[row][col].getValue());
        }

        const std::vector<int> result = processLine(line);
        if (result != line) {
            changed = true;
            for (std::size_t offset = 0; offset < boardSize; ++offset) {
                const std::size_t position = reversed ? boardSize - 1 - offset : offset;
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
    for (std::size_t row = 0; row < boardSize; ++row) {
        for (std::size_t col = 0; col < boardSize; ++col) {
            if (grid[row][col].isEmpty()) {
                emptyPositions.push_back(row * boardSize + col);
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
    grid[position / boardSize][position % boardSize].setValue(value);
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
    for (std::size_t row = 0; row < boardSize; ++row) {
        for (std::size_t col = 0; col < boardSize; ++col) {
            const int value = grid[row][col].getValue();
            if (grid[row][col].isEmpty()
                || (col + 1 < boardSize && value == grid[row][col + 1].getValue())
                || (row + 1 < boardSize && value == grid[row + 1][col].getValue())) {
                return true;
            }
        }
    }
    return false;
}

bool Board::isGameOver() const {
    return !canMove();
}

bool Board::operator==(const Board& other) const {
    if (boardSize != other.boardSize) {
        return false;
    }
    for (std::size_t row = 0; row < boardSize; ++row) {
        for (std::size_t col = 0; col < boardSize; ++col) {
            if (grid[row][col].getValue() != other.grid[row][col].getValue()) {
                return false;
            }
        }
    }
    return true;
}

bool Board::operator!=(const Board& other) const {
    return !(*this == other);
}
int Board::getHighestTile() const {
    int maxTile = 0;

    // Iterate through the entire grid to find the highest value
    for (int y = 0; y < boardSize; ++y) {
        for (int x = 0; x < boardSize; ++x) {
                int currentValue = grid[y][x].getValue();
                if (currentValue > maxTile) {
                    maxTile = currentValue;
            }
        }
    }

    return maxTile;
}
std::ostream& operator<<(std::ostream& os, const Board& board) {
    for (std::size_t row = 0; row < board.boardSize; ++row) {
        for (std::size_t col = 0; col < board.boardSize; ++col) {
            os << std::setw(6) << board.grid[row][col].getValue();
        }
        os << '\n';
    }
    return os;
}
