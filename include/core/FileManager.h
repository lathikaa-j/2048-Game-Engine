#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>

class FileManager {
public:
    // Static methods so they can be called easily by Save/Load systems
    static bool writeToFile(const std::string& filename, const std::string& content);
    static std::string readFromFile(const std::string& filename);
};

#endif