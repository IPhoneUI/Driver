#ifndef WEATHERINFO_H
#define WEATHERINFO_H

#include <string>
#include <utils/filesystem.h>
#include <weather/WeatherDef.h>

namespace driver {

class WeatherInfo {
public:
    WeatherInfo(const std::string& name, const double& lat, const double& log, const u_int8_t& des);
    ~WeatherInfo();

    operator service::WeatherParameters() {
        return mData;
    }

    void setWeatherData(const std::string& location, const double& currentTmp, const double& maxTemp, const double& minTemp);
    void fetchData(const std::string& key);
    service::LocationCoordinate getDestination() const;
    void parseData();
    std::string getRawData() const {
        return mRawData;
    }
    std::string getNameLocation() const {
        return mData.mLocationName;
    }

private:
    service::WeatherParameters mData;
    std::string mRawData;
};

}

#endif //WEATHERINFO_H
