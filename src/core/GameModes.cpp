#include "core/GameModes.h"

TargetMode::TargetMode(int target) {
    // Optionally add validation to ensure the target is a power of 2 (e.g., 64, 128, 512)
    if (target < 8 || (target & (target - 1)) != 0) {
        targetTile = 2048; // Safe fallback if the user enters something invalid
    } else {
        targetTile = target;
    }
}

bool TargetMode::isTargetReached(const Board& board) const {
    // Check if the highest tile on the board matches or exceeds the user's target
    return board.getHighestTile() >= targetTile;
}

int TargetMode::getTarget() const {
    return targetTile;
}