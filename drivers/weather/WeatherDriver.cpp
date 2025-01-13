#include "WeatherDriver.h"

namespace driver {

static WeatherDriver* gInstance = nullptr;

WeatherDriver::WeatherDriver()
{
    // mRepository.setName("locations");

    // mRepository.addParam("weatherlocations", common::ParameterIndex::Weather_Location);

    Connection::connect(mRepository.onRepoStateChanged, std::bind(&WeatherDriver::onRepoStateChanged, this, std::placeholders::_1));

    // mRepository.pull();

    common::DriverExecution::instance().addDriver("WeatherDriver", this);
}

WeatherDriver::~WeatherDriver()
{

}

void WeatherDriver::execute(milliseconds delta)
{
    // static milliseconds tempDelta = 0;
    // if (tempDelta >= 8000) {

    // }
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

void WeatherDriver::getWeatherFigures()
{

}

void WeatherDriver::readData()
{
    // utils::VariantList dataList = mRepository[common::ParameterIndex::Weather_Location];
    // for (const auto &item : dataList) {
    //     LOG_INFO("THAIVD ---- DATA:");
    // }
}

void WeatherDriver::onRepoStateChanged(common::Repository::State state)
{
    if (state == common::Repository::PullCompleted) {
        readData();
    }
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
