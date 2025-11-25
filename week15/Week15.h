#ifndef WEEK15_H
#define WEEK15_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <map>
#include <limits>
#include <cmath>
#include <algorithm>

// Structure to hold Mars weather data
struct MarsWeatherData {
    int id;
    std::string terrestrial_date;
    int sol;
    int ls;
    std::string month;
    double min_temp;
    double max_temp;
    double pressure;
    std::string wind_speed;  // Can be "NaN"
    std::string atmo_opacity;
};

// CSV parser class
class MarsWeatherCSVReader {
private:
    std::vector<MarsWeatherData> records;
    
    // Parse a single line of CSV
    MarsWeatherData parseLine(const std::string& line);

public:
    // Load CSV data from file
    bool load(const std::string& file_path);
    
    // Get first N records
    std::vector<MarsWeatherData> head(size_t n = 5);
    
    // Get number of records
    size_t size() const;
    
    // Get all records
    const std::vector<MarsWeatherData>& getRecords() const;
};

// Convert month number to month name
std::string getMonthName(int monthNum);

// Extract month number from "Month X" format
int extractMonthNumber(const std::string& monthStr);

// Print a single record
void printRecord(const MarsWeatherData& data);

#endif // WEEK15_H

