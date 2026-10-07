#include "../../include/Core/HighScoreSystem.h"
#include <iostream>
#include <algorithm>

void HighScoreSystem::registerScore(const std::string& username, int score) {
    leaderboard.push_back({username, score});
    
    // Sort in descending order based on the score
    std::sort(leaderboard.begin(), leaderboard.end(), 
              [](const LeaderboardEntry& a, const LeaderboardEntry& b) {
                  return a.score > b.score;
              });

    // Ensure the leaderboard doesn't exceed the max size
    if (leaderboard.size() > MAX_LEADERBOARD_SIZE) {
        leaderboard.pop_back();
    }
}

void HighScoreSystem::printLeaderboard() const {
    std::cout << "\n--- 2048 LEADERBOARD ---\n";
    for (size_t i = 0; i < leaderboard.size(); ++i) {
        std::cout << (i + 1) << ". " << leaderboard[i].username 
                  << " : " << leaderboard[i].score << "\n";
    }
    std::cout << "------------------------\n";
}