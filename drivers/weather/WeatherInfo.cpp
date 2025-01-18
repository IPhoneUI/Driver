#include "WeatherInfo.h"
#include <string.h>
#include <curl/curl.h>

namespace driver {

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    size_t totalSize = size * nmemb;
    userp->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

WeatherInfo::WeatherInfo(const std::string &name, const double &lat, const double &log, const u_int8_t &des)
{
    mData.mLocationName = name;
    mData.mLocationInfo = { .nameLocation = name,
                           .latitude = lat,
                      .longitude = log,
                      .defaultDes = des
                    };
}

WeatherInfo::~WeatherInfo()
{

}

void WeatherInfo::setWeatherData(const std::string &location, const double &currentTmp, const double &maxTemp, const double &minTemp)
{

}

void WeatherInfo::fetchData(const std::string &key)
{
    if (key.empty())
    {
        return;
    }

    std::string url = "https://api.openweathermap.org/data/2.5/weather?lat=" + std::to_string(mData.mLocationInfo.latitude) + "&lon="+ std::to_string(mData.mLocationInfo.longitude) +"&appid=" + key + "&units=metric";
    std::string response;

    CURL* curl;
    curl = curl_easy_init();

    if (curl == NULL) {
        return;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        LOG_WARN("[WeatherServer] - perform get curl is failed");
        return;
    }

    curl_easy_cleanup(curl);

    if (response.empty()) {
        return;
    }

    mRawData = response;

    this->parseData();
}

service::LocationCoordinate WeatherInfo::getDestination() const
{
    return mData.mLocationInfo;
}

void WeatherInfo::parseData()
{
    json j;

    j = json::parse(mRawData);

    mData.mCurrentTemp = j["main"]["temp"];
    mData.mMaxTemp = j["main"]["temp_max"];
    mData.mMinTemp = j["main"]["temp_min"];
}

}
