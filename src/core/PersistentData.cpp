#include "../../include/core/PersistentData.h"
#include <fstream>

bool PersistentData::exportToCSV(const std::string& filename, const std::vector<std::string>& headers, const std::vector<std::vector<std::string>>& data) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    // Write the CSV headers
    for (size_t i = 0; i < headers.size(); ++i) {
        file << headers[i];
        if (i < headers.size() - 1) file << ",";
    }
    file << "\n";

    // Write the data rows
    for (const auto& row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << row[i];
            if (i < row.size() - 1) file << ",";
        }
        file << "\n";
    }

    file.close();
    return true;
}

bool PersistentData::saveToTXT(const std::string& filename, const std::vector<std::pair<std::string, int>>& dataPairs) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    // Write formatted key-value pairs
    for (const auto& pair : dataPairs) {
        file << pair.first << ": " << pair.second << "\n";
    }

    file.close();
    return true;
}