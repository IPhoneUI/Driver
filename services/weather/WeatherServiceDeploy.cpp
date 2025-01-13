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

WeatherServiceDeploy::WeatherServiceDeploy()
{

}

}
