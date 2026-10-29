#pragma once
#include<cstddef>
#include "core/Board.h"
#include "core/GameState.h"
#include "utils/History.h"
#include "core/GameModes.h"
#include "core/Score_Manager.h"

enum class Direction { Left, Right, Up, Down };

class Game {
private:
    struct GameSnapshot {
        Board board;
        int score;
        GameState state;
    };

    Board board;
    ScoreManager score;
    GameModes modes;
    GameState state;
    History<GameSnapshot> history;

    void updateState();

public:
    Game();
    // Construct a session from a board snapshot (also useful for deterministic scenarios).
    explicit Game(const Board& initialBoard, int initialScore = 0, GameModes mode = GameModes{});
    const GameModes& getModes() const { return modes; }
    explicit Game(std::size_t boardSize);
    void newGame();
    void newGame(std::size_t boardSize);
    // Blocked moves return false; invalid Direction enum values throw InvalidMoveException.
    bool move(Direction direction);
    bool undo();
    void pause();
    void resume();

    const Board& getBoard() const;
    int getScore() const;
    GameState getState() const;
};
