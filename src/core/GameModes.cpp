#include "../../include/Core/GameModes.h"

void GameModes::setMode(Mode mode, int customTarget) {
    currentMode = mode;
    if (mode == Mode::TARGET) {
        targetTile = customTarget;
    } else if (mode == Mode::STANDARD) {
        targetTile = 2048;
    }
}

Mode GameModes::getCurrentMode() const {
    return currentMode;
}

bool GameModes::checkWinCondition(int highestTileOnBoard) const {
    if (currentMode == Mode::ENDLESS) {
        return false; // Endless mode never triggers a win state
    }
    
    // For Standard and Target modes, check if the target has been reached
    return highestTileOnBoard >= targetTile;
}   