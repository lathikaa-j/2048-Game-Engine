#include "../../include/core/GameHistory.h"

void GameHistory::addGameRecord(int finalScore, int highestTile, int totalMoves) {
    history.push_back({finalScore, highestTile, totalMoves});
}

std::vector<PastGame> GameHistory::getHistory() const {
    return history;
}

void GameHistory::clearHistory() {
    history.clear();
}