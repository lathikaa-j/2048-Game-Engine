#include "../../include/Core/LoadGame.h"
#include "../../include/Core/FileManager.h"
#include <sstream>

bool LoadGame::loadState(const std::string& username, int& outScore, std::string& outBoardData) {
    std::string filename = username + "_save.txt";
    std::string content = FileManager::readFromFile(filename);
    
    // If the file is empty or doesn't exist, loading fails
    if (content.empty()) {
        return false;
    }

    std::stringstream ss(content);
    std::string scoreStr;
    
    // The first line is the score
    std::getline(ss, scoreStr);
    outScore = std::stoi(scoreStr);
    
    // The rest of the file is the board data
    outBoardData.assign(std::istreambuf_iterator<char>(ss), std::istreambuf_iterator<char>());
    
    return true;
}