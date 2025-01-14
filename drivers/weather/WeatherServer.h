#ifndef WEATHERSERVER_H
#define WEATHERSERVER_H

#include <string>
#include <vector>

namespace driver
{

struct LocationInfo {
    std::string nameLocation;
    double latitude;
    double longitude;
    u_int8_t defaultDes;
};

class WeatherInfo {
public:
    WeatherInfo(const std::string& name, const double& lat, const double& log, const u_int8_t& des);
    ~WeatherInfo();

    void setWeatherData(const std::string& location, const double& currentTmp, const double& maxTemp, const double& minTemp);
    void fetchData(const std::string& key);
    LocationInfo getDestination() const;

private:
    LocationInfo mLocationInfo;
    std::string mLocationName;
    double mCurrentTemp;
    double mMaxTemp;
    double mMinTemp;
    std::string mRawData;
};

class WeatherDriver;
class WeatherServer
{
    friend class WeatherDriver;
public:
    WeatherServer(const std::string& key);
    ~WeatherServer();

    void syncData();
    void addDestination(WeatherInfo* des);

private:
    std::string mKeyAPI;
    std::vector<WeatherInfo*> mLocations;
};
}

#endif //WEATHERSERVER_H
