#include "../../include/core/FileManager.h"
#include <fstream>
#include <sstream>

bool FileManager::writeToFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << content;
        file.close();
        return true;
    }
    return false;
}

std::string FileManager::readFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    if (file.is_open()) {
        buffer << file.rdbuf();
        file.close();
    }
    return buffer.str();
}