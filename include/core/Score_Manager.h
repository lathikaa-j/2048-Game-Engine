#ifndef SCORE_MANAGER_H
#define SCORE_MANAGER_H

class ScoreManager {
private:
    int currentScore = 0;

public:
    void addMergeScore(int mergedTileValue);
    void resetScore();
    int getCurrentScore() const;
};

#endif