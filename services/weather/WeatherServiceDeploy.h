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

namespace service {
class WeatherServiceDeploy
{
public:
    static WeatherServiceDeploy* instance();
    ~WeatherServiceDeploy();


private:
    explicit WeatherServiceDeploy();
};
}

#endif //WEATHERSERVICEDEPLOY_H
