#ifndef PERSISTENT_DATA_H
#define PERSISTENT_DATA_H

#include <string>
#include <vector>
#include <utility>

class PersistentData {
public:
    // Export tabular data (like Game History) to a .csv file
    static bool exportToCSV(const std::string& filename, const std::vector<std::string>& headers, const std::vector<std::vector<std::string>>& data);
    
    // Save key-value pairs (like Statistics) to a formatted .txt file
    static bool saveToTXT(const std::string& filename, const std::vector<std::pair<std::string, int>>& dataPairs);
};

#endif