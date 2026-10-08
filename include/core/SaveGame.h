#ifndef SAVE_GAME_H
#define SAVE_GAME_H

#include <string>

class SaveGame {
public:
    // Takes the player's name, current score, and a string representing the 2048 board grid
    bool saveState(const std::string& username, int score, const std::string& boardData);
};

#endif