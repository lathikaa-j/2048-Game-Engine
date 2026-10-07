#include "core/Board.h"
#include "core/Tile.h"
#include "core/Game.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <type_traits>
#include <vector>

static_assert(std::is_same<decltype(std::declval<Game&>().getBoard()), const Board&>::value,
              "Game board access must be read-only");

struct DemoCase {
    const char* name;
    Direction direction;
    std::vector<std::vector<int>> before;
    std::vector<std::vector<int>> expected;
    bool expectedChanged;
    int expectedScore = -1; // Old movement demonstrations need no score expectation.
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
    if (demo.expectedScore >= 0) {
        passed = passed && board.getLastMoveScore() == demo.expectedScore;
        std::cout << "Score gained: " << board.getLastMoveScore()
                  << " (expected " << demo.expectedScore << ")\n";
    }
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

std::vector<std::vector<int>> snapshot(const Board& board) {
    std::vector<std::vector<int>> values(board.getSize(), std::vector<int>(board.getSize()));
    for (std::size_t row = 0; row < board.getSize(); ++row) {
        for (std::size_t col = 0; col < board.getSize(); ++col) {
            values[row][col] = board.getTile(row, col).getValue();
        }
    }
    return values;
}

void recordCheck(const char* name, bool passed, int& testsPassed, int& testsFailed) {
    std::cout << name << ": " << (passed ? "PASS" : "FAIL") << '\n';
    if (passed) {
        ++testsPassed;
    } else {
        ++testsFailed;
    }
}

void checkRandomTiles(int& testsPassed, int& testsFailed) {
    Board board;
    // Fill all 16 cells, checking every insertion and every previously occupied cell.
    for (std::size_t count = 1; count <= Board::BOARD_SIZE * Board::BOARD_SIZE; ++count) {
        const auto before = snapshot(board);
        bool passed = board.addRandomTile();
        std::size_t occupied = 0;
        std::size_t newTiles = 0;
        for (std::size_t row = 0; row < board.getSize(); ++row) {
            for (std::size_t col = 0; col < board.getSize(); ++col) {
                const int value = board.getTile(row, col).getValue();
                if (value != 0) {
                    ++occupied;
                }
                if (before[row][col] != 0) {
                    passed = passed && value == before[row][col];
                } else if (value != 0) {
                    ++newTiles;
                    passed = passed && (value == 2 || value == 4);
                }
            }
        }
        passed = passed && occupied == count && newTiles == 1;
        recordCheck("Random insertion: one new 2/4, existing tiles preserved", passed,
                    testsPassed, testsFailed);
    }
    const auto fullBoard = snapshot(board);
    const bool added = board.addRandomTile();
    recordCheck("Full board: insertion refused, board unchanged",
                !added && snapshot(board) == fullBoard, testsPassed, testsFailed);
}

void checkBoardState(int& testsPassed, int& testsFailed) {
    Board board;
    const std::vector<std::vector<int>> blocked = {
        {2, 4, 8, 16}, {32, 64, 128, 256},
        {512, 1024, 2, 4}, {8, 16, 32, 64}
    };
    for (std::size_t row = 0; row < board.getSize(); ++row) {
        for (std::size_t col = 0; col < board.getSize(); ++col) {
            board.setTile(row, col, blocked[row][col]);
        }
    }
    const Board& readOnly = board;
    recordCheck("Full board without neighbors: game over",
                !readOnly.canMove() && readOnly.isGameOver(), testsPassed, testsFailed);
    recordCheck("No winning tile", !readOnly.hasWon(), testsPassed, testsFailed);
    board.setTile(3, 3, 0);
    recordCheck("Empty cell: move available",
                readOnly.canMove() && !readOnly.isGameOver(), testsPassed, testsFailed);
    board.setTile(3, 3, 32);
    recordCheck("Only horizontal merge: move available",
                readOnly.canMove() && !readOnly.isGameOver(), testsPassed, testsFailed);
    board.setTile(3, 3, 4);
    const auto beforeQueries = snapshot(board);
    recordCheck("Only vertical merge: move available",
                readOnly.canMove() && !readOnly.isGameOver(), testsPassed, testsFailed);
    readOnly.hasWon();
    recordCheck("State queries leave board unchanged",
                snapshot(board) == beforeQueries, testsPassed, testsFailed);
    board.setTile(0, 0, 2048);
    recordCheck("2048 wins", readOnly.hasWon(), testsPassed, testsFailed);
    board.setTile(0, 0, 4096);
    recordCheck("4096 wins", readOnly.hasWon(), testsPassed, testsFailed);
    board.clear();
    recordCheck("Empty board: no win, moves available",
                !readOnly.hasWon() && readOnly.canMove() && !readOnly.isGameOver(),
                testsPassed, testsFailed);
}

bool moveBoard(Board& board, Direction direction) {
    switch (direction) {
        case Direction::Left: return board.moveLeft();
        case Direction::Right: return board.moveRight();
        case Direction::Up: return board.moveUp();
        case Direction::Down: return board.moveDown();
    }
    return false;
}

std::size_t occupiedCount(const Board& board) {
    std::size_t count = 0;
    for (const auto& row : snapshot(board)) {
        for (int value : row) {
            if (value != 0) { ++count; }
        }
    }
    return count;
}

bool initialTilesValid(const Game& game) {
    bool valid = occupiedCount(game.getBoard()) == 2;
    for (const auto& row : snapshot(game.getBoard())) {
        for (int value : row) {
            valid = valid && (value == 0 || value == 2 || value == 4);
        }
    }
    return valid;
}

void checkNewPhases(int& testsPassed, int& testsFailed) {
    std::cout << "\nPHASE 7 - GAME CONTROLLER\n";
    Game game;
    recordCheck("Initialization: exactly two 2/4 tiles", initialTilesValid(game),
                testsPassed, testsFailed);
    recordCheck("Initial score and state", game.getScore() == 0
                && game.getState() == GameState::Playing, testsPassed, testsFailed);
    const Board initial = game.getBoard();
    game.resume();
    game.pause();
    game.pause();
    recordCheck("Pause blocks all directions", game.getState() == GameState::Paused
                && !game.move(Direction::Left) && !game.move(Direction::Right)
                && !game.move(Direction::Up) && !game.move(Direction::Down)
                && game.getBoard() == initial && game.getScore() == 0 && !game.undo(),
                testsPassed, testsFailed);
    game.resume();
    game.resume();
    recordCheck("Resume returns to Playing", game.getState() == GameState::Playing,
                testsPassed, testsFailed);

    std::cout << "\nPHASE 8 - BOARD OPERATORS\n";
    Board a;
    Board b;
    a.setTile(0, 0, 2);
    b.setTile(0, 0, 2);
    recordCheck("Identical boards compare equal", a == b && !(a != b),
                testsPassed, testsFailed);
    b.setTile(3, 3, 4);
    recordCheck("One differing tile compares unequal", a != b && !(a == b),
                testsPassed, testsFailed);
    Board scored;
    scored.setTile(0, 0, 2);
    scored.setTile(0, 1, 2);
    scored.moveLeft();
    Board sameValues;
    sameValues.setTile(0, 0, 4);
    recordCheck("Equality ignores RNG and last move score", scored == sameValues,
                testsPassed, testsFailed);
    std::ostringstream printed;
    const Board beforePrint = b;
    std::ostream& streamResult = printed << b;
    recordCheck("Stream insertion: matrix, chaining, no mutation",
                &streamResult == &printed && b == beforePrint && printed.str() ==
                "     2     0     0     0\n"
                "     0     0     0     0\n"
                "     0     0     0     0\n"
                "     0     0     0     4\n", testsPassed, testsFailed);
    std::cout << "Board << demonstration:\n" << b;

    std::cout << "\nPHASE 9 - HISTORY / UNDO\n";
    History<int> numbers;
    recordCheck("History<int> initially empty", numbers.empty() && numbers.size() == 0,
                testsPassed, testsFailed);
    numbers.push(10);
    numbers.push(20);
    const History<int>& readOnlyHistory = numbers;
    recordCheck("History push/top/size", !numbers.empty() && numbers.size() == 2
                && readOnlyHistory.top() == 20, testsPassed, testsFailed);
    const int last = numbers.pop();
    recordCheck("History pop uses LIFO", last == 20 && numbers.top() == 10
                && numbers.size() == 1 && numbers.pop() == 10 && numbers.empty(),
                testsPassed, testsFailed);
    numbers.push(30);
    numbers.clear();
    recordCheck("History clear", numbers.empty() && numbers.size() == 0,
                testsPassed, testsFailed);
    bool topRejected = false;
    bool popRejected = false;
    try { numbers.top(); } catch (const std::out_of_range&) { topRejected = true; }
    try { numbers.pop(); } catch (const std::out_of_range&) { popRejected = true; }
    recordCheck("Empty History access fails safely", topRejected && popRejected
                && numbers.empty(), testsPassed, testsFailed);
    recordCheck("Empty game undo returns false", !game.undo(), testsPassed, testsFailed);

    const std::vector<Direction> directions = {
        Direction::Left, Direction::Right, Direction::Up, Direction::Down
    };
    std::vector<Board> boards;
    std::vector<int> scores;
    bool workflowValid = true;
    bool invalidValid = true;
    bool sawInvalid = false;
    bool sawMerge = false;
    // Probe copies to choose valid moves without changing Game encapsulation.
    // Continue until the first merge and no-change move have both been exercised.
    for (int turn = 0; turn < 10000 && game.getState() == GameState::Playing; ++turn) {
        bool moved = false;
        for (Direction direction : directions) {
            Board expected = game.getBoard();
            const int oldScore = game.getScore();
            if (!moveBoard(expected, direction)) {
                sawInvalid = true;
                invalidValid = invalidValid && !game.move(direction)
                    && game.getBoard() == expected && game.getScore() == oldScore
                    && game.getState() == GameState::Playing;
                continue;
            }
            if (moved) { continue; }
            boards.push_back(game.getBoard());
            scores.push_back(oldScore);
            workflowValid = game.move(direction) && workflowValid;
            const auto values = snapshot(expected);
            std::size_t newTiles = 0;
            for (std::size_t row = 0; row < expected.getSize(); ++row) {
                for (std::size_t col = 0; col < expected.getSize(); ++col) {
                    const int value = game.getBoard().getTile(row, col).getValue();
                    if (values[row][col] != 0) {
                        workflowValid = workflowValid && value == values[row][col];
                    } else if (value != 0) {
                        ++newTiles;
                        workflowValid = workflowValid && (value == 2 || value == 4);
                    }
                }
            }
            const GameState expectedState = game.getBoard().hasWon() ? GameState::Won
                : game.getBoard().isGameOver() ? GameState::Lost : GameState::Playing;
            workflowValid = workflowValid && newTiles == 1
                && occupiedCount(game.getBoard()) == occupiedCount(expected) + 1
                && game.getScore() == oldScore + expected.getLastMoveScore()
                && game.getState() == expectedState;
            sawMerge = sawMerge || expected.getLastMoveScore() > 0;
            moved = true;
            break;
        }
        if (!moved || (sawInvalid && sawMerge && boards.size() >= 3)) { break; }
    }
    recordCheck("Valid moves: scoring, one spawn, state update", workflowValid
                && sawMerge && boards.size() >= 3, testsPassed, testsFailed);
    recordCheck("No-change moves preserve board, score and state", sawInvalid && invalidValid,
                testsPassed, testsFailed);
    bool boardRestored = true;
    bool scoreRestored = true;
    bool stateRestored = true;
    for (std::size_t index = boards.size(); index > 0; --index) {
        const bool undone = game.undo();
        boardRestored = boardRestored && undone && game.getBoard() == boards[index - 1];
        scoreRestored = scoreRestored && game.getScore() == scores[index - 1];
        stateRestored = stateRestored && game.getState() == GameState::Playing;
    }
    recordCheck("Multiple undo restores boards and removes spawned tiles", boardRestored,
                testsPassed, testsFailed);
    recordCheck("Undo restores total score including merges", scoreRestored,
                testsPassed, testsFailed);
    recordCheck("Undo restores state", stateRestored, testsPassed, testsFailed);
    recordCheck("Invalid moves created no history entries", !game.undo()
                && game.getBoard() == initial, testsPassed, testsFailed);

    for (Direction direction : directions) {
        Board probe = game.getBoard();
        if (moveBoard(probe, direction)) { game.move(direction); break; }
    }
    game.pause();
    recordCheck("Undo while paused restores pre-move Playing state", game.undo()
                && game.getBoard() == initial && game.getState() == GameState::Playing,
                testsPassed, testsFailed);
    for (Direction direction : directions) {
        Board probe = game.getBoard();
        if (moveBoard(probe, direction)) { game.move(direction); break; }
    }
    game.pause();
    game.newGame();
    recordCheck("New game resets board, score, state and history", initialTilesValid(game)
                && game.getScore() == 0 && game.getState() == GameState::Playing
                && !game.undo(), testsPassed, testsFailed);
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

    int testsPassed = 0;
    int testsFailed = 0;
    std::cout << "PHASES 1-3 - REGRESSION\n";
    recordCheck("Basic classes", checkBasicClasses(), testsPassed, testsFailed);
    for (const DemoCase& demo : demos) {
        recordCheck(demo.name, runDemo(demo), testsPassed, testsFailed);
    }

    std::cout << "\nPHASE 4 - RANDOM TILE\n";
    checkRandomTiles(testsPassed, testsFailed);

    std::cout << "\nPHASE 5 - SCORING\n";
    const std::vector<DemoCase> scoreCases = {
        {"LEFT score: gaps", Direction::Left, {{2, 0, 2, 0}}, {{4, 0, 0, 0}}, true, 4},
        {"LEFT score: four equal", Direction::Left, {{2, 2, 2, 2}}, {{4, 4, 0, 0}}, true, 8},
        {"LEFT score: two pairs", Direction::Left, {{2, 2, 4, 4}}, {{4, 8, 0, 0}}, true, 12},
        {"LEFT score: larger pairs", Direction::Left, {{4, 4, 8, 8}}, {{8, 16, 0, 0}}, true, 24},
        {"LEFT score: compression", Direction::Left, {{0, 2, 4, 8}}, {{2, 4, 8, 0}}, true, 0},
        {"LEFT score: unchanged", Direction::Left, {{2, 4, 8, 16}}, {{2, 4, 8, 16}}, false, 0},
        {"RIGHT score: two pairs", Direction::Right, {{2, 2, 4, 4}}, {{0, 0, 4, 8}}, true, 12},
        {"UP score: two pairs", Direction::Up,
         {{2, 0, 0, 0}, {2, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}},
         {{4, 0, 0, 0}, {8, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, true, 12},
        {"DOWN score: two pairs", Direction::Down,
         {{2, 0, 0, 0}, {2, 0, 0, 0}, {4, 0, 0, 0}, {4, 0, 0, 0}},
         {{0, 0, 0, 0}, {0, 0, 0, 0}, {4, 0, 0, 0}, {8, 0, 0, 0}}, true, 12},
        {"Score sums multiple rows", Direction::Left,
         {{2, 2, 0, 0}, {4, 4, 0, 0}}, {{4, 0, 0, 0}, {8, 0, 0, 0}}, true, 12}
    };
    for (const DemoCase& demo : scoreCases) {
        recordCheck(demo.name, runDemo(demo), testsPassed, testsFailed);
    }
    // Seed a nonzero score before each direction's no-change/compression test.
    const std::vector<Direction> directions = {
        Direction::Left, Direction::Right, Direction::Up, Direction::Down
    };
    for (Direction direction : directions) {
        Board board;
        board.setTile(0, 0, 2);
        board.setTile(0, 1, 2);
        board.moveLeft();
        bool passed = board.getLastMoveScore() == 4;
        bool changed = false;
        switch (direction) {
            case Direction::Left: changed = board.moveLeft(); break;
            case Direction::Right: changed = board.moveRight(); break;
            case Direction::Up: changed = board.moveUp(); break;
            case Direction::Down: changed = board.moveDown(); break;
        }
        const bool expectedChanged = direction == Direction::Right || direction == Direction::Down;
        passed = passed && changed == expectedChanged && board.getLastMoveScore() == 0;
        recordCheck("Next move resets previous score", passed, testsPassed, testsFailed);
    }
    Board clearedBoard;
    clearedBoard.setTile(0, 0, 2);
    clearedBoard.setTile(0, 1, 2);
    clearedBoard.moveLeft();
    clearedBoard.clear();
    recordCheck("Clear resets move score", clearedBoard.getLastMoveScore() == 0,
                testsPassed, testsFailed);

    std::cout << "\nPHASE 6 - WIN/GAME OVER\n";
    checkBoardState(testsPassed, testsFailed);
    checkNewPhases(testsPassed, testsFailed);
    std::cout << "\nTests executed: " << testsPassed + testsFailed
              << "\nTests passed: " << testsPassed
              << "\nTests failed: " << testsFailed << '\n';
    return testsFailed == 0 ? 0 : 1;
}
