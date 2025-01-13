#ifndef WEATHERMONITOR_H
#define WEATHERMONITOR_H

#include <string>
#include <vector>

namespace driver
{
class WeatherLocation {
public:
    WeatherLocation();
    ~WeatherLocation();


    std::string mLocation;

};

class WeatherMonitor
{
public:
    WeatherMonitor(const std::string& key);
    ~WeatherMonitor();


private:
    std::string mKeyAPI;
    std::vector<WeatherLocation> mLocations;
};
}

#endif //WEATHERMONITOR_H
