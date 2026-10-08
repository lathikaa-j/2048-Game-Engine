#ifndef LOAD_GAME_H
#define LOAD_GAME_H

#include <string>

class LoadGame {
public:
    // Uses references (&) to output the loaded score and board data back to the main game
    bool loadState(const std::string& username, int& outScore, std::string& outBoardData);
};

#endif