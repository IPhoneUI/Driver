#ifndef WEATHERSERVICEIMPL_H
#define WEATHERSERVICEIMPL_H

#include <string>
#include <thread>
#include <functional>
#include <utility>
#include <common/BaseServiceImpl.h>
#include "WeatherServiceDeploy.h"
#include <weather/WeatherProvider.h>

namespace service {

class WeatherServiceImpl : public common::BaseServiceImpl
{
public:
    explicit WeatherServiceImpl();
    ~WeatherServiceImpl();

    void onMsqReceived() override;
    void initialize() override;
    void finialize() override;
    void registerClient(const std::string& clientName);

    void onWeatherDataChanged(const std::list<std::pair<std::string, driver::WeatherInfo*>>& dataList);

private:
    WeatherServiceDeploy* mDeploy {nullptr};
    driver::WeatherDriver* mDriver {nullptr};
    base::shm::WeatherProvider* mProvider {nullptr};
};

}

#endif //WEATHERSERVICEIMPL_H
