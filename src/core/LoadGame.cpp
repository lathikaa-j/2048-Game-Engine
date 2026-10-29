#include "../../include/core/LoadGame.h"
#include "../../include/core/FileManager.h"
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
    int parsedScore;
    std::istringstream scoreLine(scoreStr);
    if (!(scoreLine >> parsedScore) || parsedScore < 0) return false;
    scoreLine >> std::ws;
    if (!scoreLine.eof()) return false;
    
    // The rest of the file is the board data
    std::string board(std::istreambuf_iterator<char>(ss), {});
    if (board.empty()) return false;
    outScore = parsedScore;
    outBoardData = board;
    
    return true;
}
