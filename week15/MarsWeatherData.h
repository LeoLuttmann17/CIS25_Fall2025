#ifndef MARS_WEATHER_DATA_H
#define MARS_WEATHER_DATA_H

#include <string>

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

#endif // MARS_WEATHER_DATA_H

