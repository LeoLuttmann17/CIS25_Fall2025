#ifndef MARS_WEATHER_CSV_READER_H
#define MARS_WEATHER_CSV_READER_H

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
#include "MarsWeatherData.h"

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

#endif // MARS_WEATHER_CSV_READER_H

