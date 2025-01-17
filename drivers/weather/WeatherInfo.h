#ifndef WEATHERINFO_H
#define WEATHERINFO_H

#include <string>
#include <utils/filesystem.h>

namespace driver {

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
    std::string getRawData() const {
        return mRawData;
    }

private:
    LocationInfo mLocationInfo;
    std::string mLocationName;
    double mCurrentTemp;
    double mMaxTemp;
    double mMinTemp;
    std::string mRawData;
};

}

#endif //WEATHERINFO_H
