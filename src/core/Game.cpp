#include "core/Game.h"
#include "exceptions/GameException.h"
#include "exceptions/InvalidMoveException.h"
#include <limits>
#include <algorithm>


Game::Game() : Game(Board::BOARD_SIZE) {}

Game::Game(const Board& initialBoard, int initialScore, GameModes mode)
    : board(initialBoard), modes(mode), state(GameState::Playing) {
    score.resetScore();
    if (initialScore < 0) { throw GameException("Score cannot be negative"); }
    score.addMergeScore(initialScore);
    updateState();
}

Game::Game(std::size_t boardSize)
    : board(boardSize), state(GameState::Playing) {
    newGame();
}

void Game::newGame() {
    newGame(board.getSize());
}

void Game::newGame(std::size_t boardSize) {
    if (!Board::isAllowedSize(boardSize)) {
        throw GameException("Board size must be 3, 4, or 5");
    }
    if (board.getSize() != boardSize) {
        board = Board(boardSize);
    } else {
        board.clear();
    }
    score.resetScore();
    state = GameState::Playing;
    history.clear();
    board.addRandomTile();
    board.addRandomTile();
    updateState();
}

bool Game::move(Direction direction) {
    // An invalid enum is a caller error, unlike an ordinary blocked move.
    switch (direction) {
        case Direction::Left: case Direction::Right:
        case Direction::Up: case Direction::Down: break;
        default: throw InvalidMoveException("Unknown movement direction");
    }
    if (state != GameState::Playing) {
        return false;
    }

    // Try a copy so unsuccessful moves leave all session data untouched.
    Board nextBoard = board;
    bool changed = false;
    switch (direction) {
        case Direction::Left: changed = nextBoard.moveLeft(); break;
        case Direction::Right: changed = nextBoard.moveRight(); break;
        case Direction::Up: changed = nextBoard.moveUp(); break;
        case Direction::Down: changed = nextBoard.moveDown(); break;
    }
    if (!changed) {
        return false;
    }

    if (nextBoard.getLastMoveScore() > std::numeric_limits<int>::max() - getScore())
        throw GameException("Score exceeds the supported integer range");
    history.push(GameSnapshot{board, getScore(), state});
    board = nextBoard;
    score.addMergeScore(board.getLastMoveScore());
    board.addRandomTile();
    updateState();
    return true;
}

void Game::updateState() {
    int highest = 0;
    for (std::size_t r = 0; r < board.getSize(); ++r)
        for (std::size_t c = 0; c < board.getSize(); ++c)
            highest = std::max(highest, board.getTile(r,c).getValue());
    if (modes.checkWinCondition(highest)) {
        state = GameState::Won;
    } else if (board.isGameOver()) {
        state = GameState::Lost;
    } else {
        state = GameState::Playing;
    }
}

bool Game::undo() {
    if (history.empty()) {
        return false;
    }
    const GameSnapshot previous = history.pop();
    board = previous.board;
    score.resetScore();
    score.addMergeScore(previous.score);
    state = previous.state;
    return true;
}

void Game::pause() {
    if (state == GameState::Playing) {
        state = GameState::Paused;
    }
}

void Game::resume() {
    if (state == GameState::Paused) {
        state = GameState::Playing;
    }
}

const Board& Game::getBoard() const { return board; }
int Game::getScore() const { return score.getCurrentScore(); }
GameState Game::getState() const { return state; }
