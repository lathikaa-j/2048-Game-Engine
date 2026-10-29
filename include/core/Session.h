#pragma once
#include "core/Game.h"
#include "core/PlayerSystem.h"
#include "core/HighScoreSystem.h"
#include "core/Statistics.h"
#include <map>
#include <cstddef>

// Coordinates modules; Game remains the sole owner of board, score and undo.
class Session {
    Game game;
    PlayerSystem players;
    GameModes selectedMode;
    std::size_t selectedBoardSize = Board::BOARD_SIZE;
    bool obstaclesEnabled = false;
    HighScoreSystem scores;
    std::map<std::string, Statistics> statistics;
    std::string owner = "Guest";
    bool started = false;
    void finish();
public:
    const Game& getGame() const { return game; }
    Game& getGame() { return game; }
    PlayerSystem& getPlayers() { return players; }
    const HighScoreSystem& getScores() const { return scores; }
    const Statistics& getStatistics(const std::string& name) { return statistics[name]; }
    std::string activePlayer() const;
    const std::string& sessionPlayer() const { return owner; }
    bool hasGame() const { return started; }
    const GameModes& getSelectedMode() const { return selectedMode; }
    void selectMode(Mode mode, int target = 2048) { selectedMode.setMode(mode, target); }
    std::size_t getSelectedBoardSize() const { return selectedBoardSize; }
    void selectBoardSize(std::size_t size);
    bool getObstaclesEnabled() const { return obstaclesEnabled; }
    void setObstaclesEnabled(bool enabled) { obstaclesEnabled = enabled; }
    void start();
    void restart();
    void observeMove(const Board& before);
    void close() { finish(); }
    bool save(std::string& message) const;
    bool load(std::string& message);
    static int highestTile(const Board& board);
};
