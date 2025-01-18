#ifndef WEATHERSERVICEDEPLOY_H
#define WEATHERSERVICEDEPLOY_H

#include <string>
#include <thread>
#include <list>
#include <functional>
#include <mutex>
#include <shared_mutex>
#include <common/BaseDeploy.h>
#include <weather/WeatherDef.h>
#include <WeatherDriver.h>

namespace service {
class WeatherServiceDeploy : public common::BaseDeploy
{
public:
    static WeatherServiceDeploy* instance();
    ~WeatherServiceDeploy();

    void responseServiceReady(const std::string& clientName) override;

    void responseWeatherDataChanged();


private:
    std::mutex mMutex;
    explicit WeatherServiceDeploy();
};
}

#endif //WEATHERSERVICEDEPLOY_H
