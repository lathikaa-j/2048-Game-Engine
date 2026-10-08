#ifndef GAME_HISTORY_H
#define GAME_HISTORY_H

#include <vector>

struct PastGame {
    int finalScore;
    int highestTile;
    int totalMoves;
};

class GameHistory {
private:
    std::vector<PastGame> history;

public:
    void addGameRecord(int finalScore, int highestTile, int totalMoves);
    std::vector<PastGame> getHistory() const;
    void clearHistory();
};

#endif