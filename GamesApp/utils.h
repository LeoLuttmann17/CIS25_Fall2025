#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <algorithm>

// Simple CSV line parser (handles quoted commas)
inline std::vector<std::string> splitCSVLine(const std::string& line) {
    std::vector<std::string> cols;
    std::string cur;
    bool inQuotes = false;

    for (char c : line) {
        if (c == '"') {
            inQuotes = !inQuotes;
            continue;
        }
        if (c == ',' && !inQuotes) {
            cols.push_back(cur);
            cur.clear();
        } else {
            cur.push_back(c);
        }
    }
    cols.push_back(cur);
    return cols;
}

// Convert string to lowercase
inline std::string toLower(const std::string& s) {
    std::string result = s;
    std::transform(result.begin(), result.end(),
                   result.begin(), ::tolower);
    return result;
}

// Trim whitespace
inline std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\n\r");
    return s.substr(start, end - start + 1);
}

#endif
