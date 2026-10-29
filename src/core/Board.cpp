#include "core/Board.h"
#include<stdexcept>
#include <iomanip>
#include <ostream>
#include <limits>
#include "exceptions/GameException.h"

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
    if (value < -1) throw std::invalid_argument("Tile value must be nonnegative or an obstacle (-1)");
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
    std::vector<int> result(boardSize, 0);
    std::size_t start = 0;
    while (start < boardSize) {
        const Tile first(line[start]);
        if (!first.isMovable()) {
            result[start] = line[start];
            ++start;
            continue;
        }
        std::size_t end = start;
        while (end < boardSize && Tile(line[end]).isMovable()) ++end;

        std::vector<int> values;
        for (std::size_t i = start; i < end; ++i)
            if (line[i] != 0) values.push_back(line[i]);
        std::size_t output = start;
        for (std::size_t i = 0; i < values.size(); ++i) {
            const Tile current(values[i]);
            const Tile next(i + 1 < values.size() ? values[i + 1] : 0);
            if (i + 1 < values.size() && current.canMergeWith(&next)) {
                if (values[i] > std::numeric_limits<int>::max()/2
                    || values[i]*2 > std::numeric_limits<int>::max()-lastMoveScore)
                throw GameException("Tile or merge score exceeds the supported integer range");
                const int mergedValue = values[i] * 2;
                result[output++] = mergedValue;
                lastMoveScore += mergedValue;
                ++i; // Consume both original tiles; the result cannot merge again.
            } else {
                result[output++] = values[i];
            }
        }
        start = end;
    }
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

bool Board::addRandomObstacle() {
    std::vector<std::size_t> emptyPositions;
    for (std::size_t row = 0; row < boardSize; ++row) {
        for (std::size_t col = 0; col < boardSize; ++col) {
            if (grid[row][col].isEmpty()) emptyPositions.push_back(row * boardSize + col);
        }
    }
    if (emptyPositions.empty()) return false;
    std::uniform_int_distribution<std::size_t> distribution(0, emptyPositions.size() - 1);
    const std::size_t position = emptyPositions[distribution(randomGenerator)];
    grid[position / boardSize][position % boardSize].setValue(-1);
    return true;
}

std::size_t Board::getObstacleCount() const {
    std::size_t count = 0;
    for (const auto& row : grid)
        for (const Tile& tile : row)
            if (tile.getValue() == -1) ++count;
    return count;
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
            if (value == 0
                || (value > 0 && col + 1 < boardSize && value == grid[row][col + 1].getValue())
                || (value > 0 && row + 1 < boardSize && value == grid[row + 1][col].getValue())) {
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
