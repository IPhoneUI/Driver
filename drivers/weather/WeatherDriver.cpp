#include "WeatherDriver.h"

#define API_KEY "a2c3652a9508a002062abb68beb71f7d"
#define SYNC_INTERVAL 300000

namespace driver {

static WeatherDriver* gInstance = nullptr;

WeatherServer::WeatherServer(const std::string &key)
    : mKeyAPI(key)
{

}

WeatherServer::~WeatherServer()
{

}

void WeatherServer::syncData()
{

}

void WeatherServer::addDestination(WeatherInfo *des)
{
    if (des == nullptr) {
        LOG_WARN("WeatherServer - weather object is null");
        return;
    }
    mLocationInfoList.emplace_back(std::make_pair(des->getNameLocation(), des));
}

// =====================================================

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
    mSyncTimer += delta;
    if (mSyncTimer.count() > SYNC_INTERVAL) {
        std::unique_lock<std::mutex> lock(mMutex);
        for (auto it : mMonitor->mLocationInfoList) {
            std::get<1>(it)->fetchData(API_KEY);
        }
        mSyncTimer = milliseconds(0);
        onWeatherDataChanged.emit(mMonitor->mLocationInfoList);
    }
}

void WeatherDriver::connectDriver()
{

}

void WeatherDriver::writeBuffer()
{
    utils::VariantList locations;
    for (auto it : mMonitor->mLocationInfoList) {
        std::unordered_map<std::string, utils::Variant> item;
        service::LocationCoordinate info = std::get<1>(it)->getDestination();
        item["location"] = info.nameLocation;
        item["latitude"] = info.latitude;
        item["longitude"] = info.longitude;
        item["default"] = static_cast<int>(info.defaultDes);
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
        std::unordered_map<std::string, utils::Variant> item = it;
        std::string name = item["location"];
        double lat = item["latitude"];
        double log = item["longitude"];
        int _default = item["default"];
        WeatherInfo* newItem = new WeatherInfo(name, lat, log, static_cast<uint8_t>(_default));
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
