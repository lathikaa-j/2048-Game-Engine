#include "core/GameModes.h"
#include <stdexcept>

void GameModes::setMode(Mode mode, int customTarget) {
    if (mode != Mode::STANDARD && mode != Mode::ENDLESS && mode != Mode::TARGET)
        throw std::invalid_argument("Unknown game mode");
    if (mode == Mode::TARGET &&
        (customTarget < 2 || customTarget > 1048576 || (customTarget & (customTarget - 1))))
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
        return false;
    }
    return highestTileOnBoard >= targetTile;
}

TargetMode::TargetMode(int target)
    : targetTile(target >= 8 && target <= 1048576 && (target & (target - 1)) == 0
                     ? target
                     : 2048) {}

bool TargetMode::isTargetReached(const Board& board) const {
    return board.getHighestTile() >= targetTile;
}

int TargetMode::getTarget() const {
    return targetTile;
}
