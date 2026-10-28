#include "../../include/Core/Statistics.h"
#include <algorithm>

void Statistics::incrementMoves() {
    totalMoves++;
}

void Statistics::updateBestTile(int tileValue) {
    // Only updates if the new tile is higher than the current best
    bestTile = std::max(bestTile, tileValue);
}

void Statistics::incrementGamesPlayed() {
    gamesPlayed++;
}

int Statistics::getMoves() const {
    return totalMoves;
}

int Statistics::getBestTile() const {
    return bestTile;
}

int Statistics::getGamesPlayed() const {
    return gamesPlayed;
}