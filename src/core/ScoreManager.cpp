#include "../../include/core/Score_Manager.h"

void ScoreManager::addMergeScore(int mergedTileValue) {
    currentScore += mergedTileValue;
}

void ScoreManager::resetScore() {
    currentScore = 0;
}

int ScoreManager::getCurrentScore() const {
    return currentScore;
}