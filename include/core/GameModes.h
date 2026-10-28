#ifndef GAME_MODES_H
#define GAME_MODES_H

enum class Mode {
    STANDARD,
    ENDLESS,
    TARGET
};

class GameModes {
private:
    Mode currentMode = Mode::STANDARD;
    int targetTile = 2048; // Default win condition for Standard mode

public:
    int getTargetTile() const { return targetTile; }
    void setMode(Mode mode, int customTarget = 2048);
    Mode getCurrentMode() const;
    bool checkWinCondition(int highestTileOnBoard) const;
};

#endif
