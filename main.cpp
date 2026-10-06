#include "core/Board.h"
#include "core/Tile.h"

#include <iomanip>
#include <iostream>
#include <vector>

// Temporary deterministic demonstrations for Phases 1-3.
enum class Direction { Left, Right, Up, Down };

struct DemoCase {
    const char* name;
    Direction direction;
    std::vector<std::vector<int>> before;
    std::vector<std::vector<int>> expected;
    bool expectedChanged;
};

void printBoard(const Board& board) {
    for (std::size_t row = 0; row < board.getSize(); ++row) {
        for (std::size_t col = 0; col < board.getSize(); ++col) {
            std::cout << std::setw(5) << board.getTile(row, col).getValue();
        }
        std::cout << '\n';
    }
}

bool runDemo(const DemoCase& demo) {
    Board board;
    for (std::size_t row = 0; row < demo.before.size(); ++row) {
        for (std::size_t col = 0; col < demo.before[row].size(); ++col) {
            board.setTile(row, col, demo.before[row][col]);
        }
    }

    std::cout << "\n" << demo.name << "\nBefore:\n";
    printBoard(board);
    bool changed = false;
    switch (demo.direction) {
        case Direction::Left: changed = board.moveLeft(); break;
        case Direction::Right: changed = board.moveRight(); break;
        case Direction::Up: changed = board.moveUp(); break;
        case Direction::Down: changed = board.moveDown(); break;
    }
    std::cout << "After:\n";
    printBoard(board);

    bool passed = changed == demo.expectedChanged;
    for (std::size_t row = 0; row < board.getSize(); ++row) {
        for (std::size_t col = 0; col < board.getSize(); ++col) {
            // Omitted rows in a demo are expected to remain empty.
            const int expected = row < demo.expected.size() ? demo.expected[row][col] : 0;
            if (board.getTile(row, col).getValue() != expected) {
                passed = false;
            }
        }
    }
    std::cout << "Changed: " << std::boolalpha << changed
              << " (expected " << demo.expectedChanged << ")\n"
              << (passed ? "PASS" : "FAIL") << '\n';
    return passed;
}

bool checkBasicClasses() {
    Tile tile;
    bool passed = tile.isEmpty() && tile.getValue() == 0;
    tile.setValue(8);
    const Tile numberedTile(4);
    passed = passed && !tile.isEmpty() && tile.getValue() == 8
             && numberedTile.getValue() == 4 && !numberedTile.isEmpty();

    Board board;
    passed = passed && board.getSize() == Board::BOARD_SIZE;
    for (std::size_t row = 0; row < board.getSize(); ++row) {
        for (std::size_t col = 0; col < board.getSize(); ++col) {
            passed = passed && board.getTile(row, col).isEmpty();
            board.setTile(row, col, 2);
        }
    }
    Tile copy = board.getTile(0, 0);
    copy.setValue(16);
    passed = passed && board.getTile(0, 0).getValue() == 2;
    board.clear();
    for (std::size_t row = 0; row < board.getSize(); ++row) {
        for (std::size_t col = 0; col < board.getSize(); ++col) {
            passed = passed && board.getTile(row, col).isEmpty();
        }
    }
    std::cout << "Tile construction/access, board initialization/clear, tile-copy isolation: "
              << (passed ? "PASS" : "FAIL") << '\n';
    return passed;
}

int main() {
    const std::vector<DemoCase> demos = {
        {"LEFT: gaps", Direction::Left, {{2, 0, 2, 0}}, {{4, 0, 0, 0}}, true},
        {"LEFT: four equal tiles", Direction::Left, {{2, 2, 2, 2}}, {{4, 4, 0, 0}}, true},
        {"LEFT: two pairs", Direction::Left, {{2, 2, 4, 4}}, {{4, 8, 0, 0}}, true},
        {"LEFT: three equal tiles", Direction::Left, {{2, 2, 2, 0}}, {{4, 2, 0, 0}}, true},
        {"LEFT: no double merge", Direction::Left, {{2, 2, 4, 0}}, {{4, 4, 0, 0}}, true},
        {"LEFT: larger pairs", Direction::Left, {{4, 4, 8, 8}}, {{8, 16, 0, 0}}, true},
        {"LEFT: four fours", Direction::Left, {{4, 4, 4, 4}}, {{8, 8, 0, 0}}, true},
        {"RIGHT: gaps", Direction::Right, {{2, 0, 2, 0}}, {{0, 0, 0, 4}}, true},
        {"RIGHT: no double merge", Direction::Right, {{0, 4, 2, 2}}, {{0, 0, 4, 4}}, true},
        {"UP: columns with gaps, pairs and triples", Direction::Up,
         {{2, 2, 2, 4}, {0, 2, 2, 0}, {2, 2, 4, 0}, {0, 2, 0, 0}},
         {{4, 4, 4, 4}, {0, 4, 4, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, true},
        {"DOWN: columns merge from the bottom", Direction::Down,
         {{2, 2, 0, 4}, {0, 2, 4, 0}, {2, 2, 2, 0}, {0, 2, 2, 0}},
         {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 4, 4, 0}, {4, 4, 4, 4}}, true},
        {"LEFT: compressed distinct values", Direction::Left,
         {{4, 8, 16, 32}}, {{4, 8, 16, 32}}, false},
        {"RIGHT: already at right edge", Direction::Right,
         {{0, 0, 2, 4}}, {{0, 0, 2, 4}}, false},
        {"UP: already at top edge", Direction::Up,
         {{2, 4, 8, 16}}, {{2, 4, 8, 16}}, false},
        {"DOWN: already at bottom edge", Direction::Down,
         {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {2, 4, 8, 16}},
         {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {2, 4, 8, 16}}, false},
        {"LEFT: compression without merging", Direction::Left,
         {{0, 2, 0, 4}}, {{2, 4, 0, 0}}, true},
        {"LEFT: empty board", Direction::Left, {}, {}, false}
    };

    bool allPassed = checkBasicClasses();
    for (const DemoCase& demo : demos) {
        const bool passed = runDemo(demo);
        allPassed = allPassed && passed;
    }
    std::cout << "\n" << (allPassed ? "All checks passed." : "Some checks failed.") << '\n';
    return allPassed ? 0 : 1;
}
