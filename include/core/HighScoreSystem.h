#ifndef HIGH_SCORE_SYSTEM_H
#define HIGH_SCORE_SYSTEM_H

#include <string>
#include <vector>

struct LeaderboardEntry {
    std::string username;
    int score;
};

class HighScoreSystem {
private:
    std::vector<LeaderboardEntry> leaderboard;
    const size_t MAX_LEADERBOARD_SIZE = 10;

public:
    void registerScore(const std::string& username, int score);
    void printLeaderboard() const;
};

#endif