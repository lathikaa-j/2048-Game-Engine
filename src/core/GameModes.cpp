#include "../../include/Core/GameModes.h"
#include <stdexcept>

void GameModes::setMode(Mode mode, int customTarget) {
    if (mode != Mode::STANDARD && mode != Mode::ENDLESS && mode != Mode::TARGET)
        throw std::invalid_argument("Unknown game mode");
    if (mode == Mode::TARGET && (customTarget < 2 || customTarget > 1048576 || (customTarget & (customTarget - 1))))
        throw std::invalid_argument("Target must be a power of two from 2 to 1048576");
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
