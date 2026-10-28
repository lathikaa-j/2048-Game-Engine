#include "../../include/Core/SaveGame.h"
#include "../../include/Core/FileManager.h"

bool SaveGame::saveState(const std::string& username, int score, const std::string& boardData) {
    // Creates a unique save file for the specific player
    std::string filename = username + "_save.txt";
    
    // Formats the data: Score on the first line, board data on the following lines
    std::string content = std::to_string(score) + "\n" + boardData;
    
    return FileManager::writeToFile(filename, content);
}