#pragma once

#include "core/Board.h"
#include "core/GameState.h"
#include "utils/History.h"

enum class Direction { Left, Right, Up, Down };

class Game {
private:
    struct GameSnapshot {
        Board board;
        int score;
        GameState state;
    };

    Board board;
    int score;
    GameState state;
    History<GameSnapshot> history;

    void updateState();

public:
    Game();
    void newGame();
    // Blocked moves return false; invalid Direction enum values throw InvalidMoveException.
    bool move(Direction direction);
    bool undo();
    void pause();
    void resume();

    const Board& getBoard() const;
    int getScore() const;
    GameState getState() const;
};
