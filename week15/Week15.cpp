#include <MarsWeatherData.h>
#include <MarsWeatherCSVReader.h>


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
    MarsWeatherData parseLine(const std::string& line) {
        MarsWeatherData data;
        std::stringstream ss(line);
        std::string cell;
        std::vector<std::string> cells;
        
        // Parse CSV line (handles commas)
        while (std::getline(ss, cell, ',')) {
            cells.push_back(cell);
        }
        
        // Parse each field
        if (cells.size() >= 10) {
            data.id = std::stoi(cells[0]);
            data.terrestrial_date = cells[1];
            data.sol = std::stoi(cells[2]);
            data.ls = std::stoi(cells[3]);
            data.month = cells[4];
            
            // Handle min_temp (can be negative or NaN)
            if (cells[5] == "NaN" || cells[5].empty()) {
                data.min_temp = std::numeric_limits<double>::quiet_NaN();
            } else {
                data.min_temp = std::stod(cells[5]);
            }
            
            // Handle max_temp (can be negative or NaN)
            if (cells[6] == "NaN" || cells[6].empty()) {
                data.max_temp = std::numeric_limits<double>::quiet_NaN();
            } else {
                data.max_temp = std::stod(cells[6]);
            }
            
            // Handle pressure
            data.pressure = std::stod(cells[7]);
            
            // Handle wind_speed (can be "NaN")
            data.wind_speed = cells[8];
            
            // Handle atmo_opacity
            data.atmo_opacity = cells[9];
        }
        
        return data;
    }

public:
    bool load(const std::string& file_path) {
        std::ifstream file(file_path);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file: " << file_path << std::endl;
            return false;
        }
        
        std::string line;
        bool first_line = true;  // Skip header
        
        while (std::getline(file, line)) {
            if (first_line) {
                first_line = false;
                continue;  // Skip header row
            }
            
            if (!line.empty()) {
                try {
                    MarsWeatherData data = parseLine(line);
                    records.push_back(data);
                } catch (const std::exception& e) {
                    std::cerr << "Warning: Could not parse line: " << line << std::endl;
                }
            }
        }
        
        file.close();
        return true;
    }
    
    // Get first N records
    std::vector<MarsWeatherData> head(size_t n = 5) {
        std::vector<MarsWeatherData> result;
        size_t count = (n < records.size()) ? n : records.size();
        for (size_t i = 0; i < count; i++) {
            result.push_back(records[i]);
        }
        return result;
    }
    
    size_t size() const {
        return records.size();
    }
    
    const std::vector<MarsWeatherData>& getRecords() const {
        return records;
    }
};

// Convert month number to month name
std::string getMonthName(int monthNum) {
    const std::string monthNames[] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };
    if (monthNum >= 1 && monthNum <= 12) {
        return monthNames[monthNum - 1];
    }
    return "Unknown";
}

// Extract month number from "Month X" format
int extractMonthNumber(const std::string& monthStr) {
    // Format is "Month X" where X is 1-12
    if (monthStr.length() > 6 && monthStr.substr(0, 6) == "Month ") {
        try {
            return std::stoi(monthStr.substr(6));
        } catch (...) {
            return 0;
        }
    }
    return 0;
}

// Print a single record
void printRecord(const MarsWeatherData& data) {
    std::cout << "ID: " << data.id 
              << ", Date: " << data.terrestrial_date
              << ", Sol: " << data.sol
              << ", LS: " << data.ls
              << ", Month: " << data.month
              << ", Min Temp: " << std::fixed << std::setprecision(1) << data.min_temp << "°C"
              << ", Max Temp: " << data.max_temp << "°C"
              << ", Pressure: " << std::setprecision(0) << data.pressure << " Pa"
              << ", Wind Speed: " << data.wind_speed
              << ", Atmo Opacity: " << data.atmo_opacity
              << std::endl;
}

int main() {
    // Path to the Mars weather CSV file
    std::string file_path = "/Users/leo/Downloads/mars-weather.csv";
    
    // Load the dataset
    MarsWeatherCSVReader reader;
    std::cout << "Loading Mars weather data from: " << file_path << std::endl;
    
    if (!reader.load(file_path)) {
        std::cerr << "Failed to load the CSV file." << std::endl;
        return 1;
    }
    
    std::cout << "Successfully loaded " << reader.size() << " records." << std::endl;
    std::cout << std::endl;
    
    // Calculate average temperatures by month
    if (reader.size() > 0) {
        // Map to store sum and count for each month
        std::map<std::string, std::pair<double, double>> month_min_sums;  // month -> (sum, count)
        std::map<std::string, std::pair<double, double>> month_max_sums;  // month -> (sum, count)
        
        // Accumulate temperatures by month (skip NaN values)
        for (const auto& record : reader.getRecords()) {
            // Update min temp statistics (only if not NaN)
            if (!std::isnan(record.min_temp)) {
                if (month_min_sums.find(record.month) == month_min_sums.end()) {
                    month_min_sums[record.month] = std::make_pair(0.0, 0.0);
                }
                month_min_sums[record.month].first += record.min_temp;
                month_min_sums[record.month].second += 1.0;
            }
            
            // Update max temp statistics (only if not NaN)
            if (!std::isnan(record.max_temp)) {
                if (month_max_sums.find(record.month) == month_max_sums.end()) {
                    month_max_sums[record.month] = std::make_pair(0.0, 0.0);
                }
                month_max_sums[record.month].first += record.max_temp;
                month_max_sums[record.month].second += 1.0;
            }
        }
        
        // Create a vector of month numbers and their data for sorting
        struct MonthData {
            int monthNum;
            std::string monthKey;
            double avg_min;
            double avg_max;
            int record_count;
        };
        
        std::vector<MonthData> monthDataList;
        
        // Process each month and convert to MonthData
        for (const auto& entry : month_min_sums) {
            const std::string& monthKey = entry.first;
            int monthNum = extractMonthNumber(monthKey);
            
            if (monthNum > 0) {
                MonthData data;
                data.monthNum = monthNum;
                data.monthKey = monthKey;
                
                // Calculate average min temp
                if (entry.second.second > 0) {
                    data.avg_min = entry.second.first / entry.second.second;
                } else {
                    data.avg_min = 0.0;
                }
                
                // Calculate average max temp
                int min_record_count = static_cast<int>(entry.second.second);
                int max_record_count = 0;
                if (month_max_sums.find(monthKey) != month_max_sums.end() && 
                    month_max_sums[monthKey].second > 0) {
                    data.avg_max = month_max_sums[monthKey].first / month_max_sums[monthKey].second;
                    max_record_count = static_cast<int>(month_max_sums[monthKey].second);
                } else {
                    data.avg_max = 0.0;
                }
                
                data.record_count = std::max(min_record_count, max_record_count);
                monthDataList.push_back(data);
            }
        }
        
        // Sort by month number (ascending order: 1-12)
        std::sort(monthDataList.begin(), monthDataList.end(), 
                  [](const MonthData& a, const MonthData& b) {
                      return a.monthNum < b.monthNum;
                  });
        
        // Print average temperatures by month
        std::cout << "Average Minimum and Maximum Temperatures by Month:" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << std::left << std::setw(15) << "Month" 
                  << std::right << std::setw(20) << "Avg Min Temp (°C)"
                  << std::right << std::setw(20) << "Avg Max Temp (°C)"
                  << std::right << std::setw(15) << "Records" << std::endl;
        std::cout << "--------------------------------------------------------" << std::endl;
        
        for (const auto& data : monthDataList) {
            std::string monthName = getMonthName(data.monthNum);
            std::cout << std::left << std::setw(15) << monthName
                      << std::right << std::fixed << std::setprecision(2) 
                      << std::setw(20) << data.avg_min
                      << std::setw(20) << data.avg_max
                      << std::setw(15) << data.record_count << std::endl;
        }
    }
    
    return 0;
}

