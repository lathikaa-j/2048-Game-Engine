#include "core/Game.h"
#include "exceptions/InvalidMoveException.h"

Game::Game() { newGame(); }

void Game::newGame() {
    board.clear();
    score = 0;
    state = GameState::Playing;
    history.clear();
    board.addRandomTile();
    board.addRandomTile();
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

    history.push(GameSnapshot{board, score, state});
    board = nextBoard;
    score += board.getLastMoveScore();
    board.addRandomTile();
    updateState();
    return true;
}

void Game::updateState() {
    if (board.hasWon()) {
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
    score = previous.score;
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
int Game::getScore() const { return score; }
GameState Game::getState() const { return state; }
