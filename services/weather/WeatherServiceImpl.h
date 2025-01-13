#ifndef WEATHERSERVICEIMPL_H
#define WEATHERSERVICEIMPL_H

#include <string>
#include <thread>
#include <functional>
#include <utility>
#include <common/BaseServiceImpl.h>
#include "WeatherServiceDeploy.h"

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

private:
    WeatherServiceDeploy* mDeploy;
};
}

#endif //WEATHERSERVICEIMPL_H
