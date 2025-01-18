#include "WeatherServiceDeploy.h"

namespace service {

static WeatherServiceDeploy* instanceObj = nullptr;

WeatherServiceDeploy *WeatherServiceDeploy::instance()
{
    if (instanceObj == nullptr) {
        instanceObj = new WeatherServiceDeploy();
    }
    return instanceObj;
}

WeatherServiceDeploy::~WeatherServiceDeploy()
{

}

void WeatherServiceDeploy::responseServiceReady(const std::string &clientName)
{
    std::lock_guard<std::mutex> lock(mMutex);
    mMqSender.startMsq(base::msq::Msq_Weather_RespServiceReady);
    mMqSender.sendMsq(clientName);
}

void WeatherServiceDeploy::responseWeatherDataChanged()
{
    mClientManager.deploy([this](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Msq_SysSett_RespAirPlaneModeUpdated);
        mMqSender.sendMsq(mqName);
    });
}

WeatherServiceDeploy::WeatherServiceDeploy()
{

}

}
