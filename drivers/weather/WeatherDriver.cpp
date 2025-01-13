#include "WeatherDriver.h"

namespace driver {

static WeatherDriver* gInstance = nullptr;

WeatherDriver::WeatherDriver()
{
    common::DriverExecution::instance().addDriver("WeatherDriver", this);
}

WeatherDriver::~WeatherDriver()
{

}

void WeatherDriver::execute(milliseconds delta)
{
    LOG_INFO("THAIVD -- WeatherDriver");
}

void WeatherDriver::connectDriver()
{

}

void WeatherDriver::writeBuffer()
{

}

void WeatherDriver::onSimulateReceived(const std::string &topic, const std::string &option, const std::string &content)
{

}


WeatherDriver *WeatherDriver::instance()
{
    if (gInstance == nullptr)
    {
        throw std::runtime_error("The WeatherDriver has not been initialized yet");
    }
    return gInstance;
}

void WeatherDriver::initialize()
{
    if (gInstance == nullptr)
    {
        gInstance = new WeatherDriver();
    }
}

}
