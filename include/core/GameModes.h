#ifndef GAME_MODES_H
#define GAME_MODES_H

#include "Board.h"

enum class Mode {
    STANDARD,
    ENDLESS,
    TARGET
};

class GameModes {
private:
    Mode currentMode = Mode::STANDARD;
    int targetTile = 2048;

public:
    int getTargetTile() const { return targetTile; }
    void setMode(Mode mode, int customTarget = 2048);
    Mode getCurrentMode() const;
    bool checkWinCondition(int highestTileOnBoard) const;
};

// Retained for callers using the standalone target mode introduced upstream.
class TargetMode {
private:
    int targetTile;

public:
    explicit TargetMode(int target);
    bool isTargetReached(const Board& board) const;
    int getTarget() const;
};

#endif
