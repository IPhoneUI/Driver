#include "WeatherDriver.h"

#define API_KEY "a2c3652a9508a002062abb68beb71f7d"

namespace driver {

static WeatherDriver* gInstance = nullptr;

WeatherDriver::WeatherDriver()
    : mMonitor(new WeatherServer(API_KEY))
{
    mRepository.setName("locations");

    mRepository.addParam("destinations", common::ParameterIndex::Weather_Locations);

    Connection::connect(mRepository.onRepoStateChanged, std::bind(&WeatherDriver::onRepoStateChanged, this, std::placeholders::_1));

    mRepository.pull();

    common::DriverExecution::instance().addDriver("WeatherDriver", this);
}

WeatherDriver::~WeatherDriver()
{

}

void WeatherDriver::execute(milliseconds delta)
{
    if (isCheck == false) {
        mTimer += delta;

        if (mTimer.count() > 10000) {
            for (auto it : mMonitor->mLocations) {
                it->fetchData(mMonitor->mKeyAPI);
                isCheck = true;
                mTimer = milliseconds(0);
            }

        }
    }
}

void WeatherDriver::connectDriver()
{

}

void WeatherDriver::writeBuffer()
{
    utils::VariantList locations;
    for (int i = 0; i < 2; i++) {
        std::unordered_map<std::string, utils::Variant> item;
        item["location"] = std::string("nghean");
        item["latitude"] = 19.274220;
        item["longitude"] = 104.840622;
        item["default"] = 1;
        utils::VariantObj obj = item;
        locations.push(obj);
    }
    mRepository[common::ParameterIndex::Weather_Locations] = locations;

    mRepository.push();
}

void WeatherDriver::onSimulateReceived(const std::string &topic, const std::string &option, const std::string &content)
{

}

void WeatherDriver::getWeatherFigures()
{

}

void WeatherDriver::readData()
{
    utils::VariantList dataList = mRepository[common::ParameterIndex::Weather_Locations];
    for (const auto &it : dataList) {
        LOG_INFO("THAIVD --- READ DATA");
        std::unordered_map<std::string, utils::Variant> item = it;
        std::string name = item["location"];
        double lat = item["latitude"];
        double lg = item["longitude"];
        int _default = item["default"];
        WeatherInfo* newItem = new WeatherInfo(name, lat, lg, static_cast<uint8_t>(_default));
        mMonitor->addDestination(newItem);
    }
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
