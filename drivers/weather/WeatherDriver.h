#ifndef WEATHERDRIVER_H
#define WEATHERDRIVER_H

#include <string>
#include <thread>
#include <functional>
#include <string>
#include <vector>
#include "WeatherInfo.h"
#include <common/BaseDriver.h>
#include <list>
#include <utility>

namespace driver {

enum class WeatherField {

};

class WeatherDriver;
class WeatherServer
{
    friend class WeatherDriver;
public:
    WeatherServer(const std::string& key);
    ~WeatherServer();

    void syncData();
    void addDestination(WeatherInfo* des);
    bool parseRawData(const std::string& data);

private:
    std::string mKeyAPI;
    std::list<std::pair<std::string, WeatherInfo*>> mLocationInfoList;
};

class WeatherDriver : public common::BaseDriver
{
public:
    static WeatherDriver* instance();
    static void initialize();
    ~WeatherDriver();

    void execute(milliseconds delta) override;
    void connectDriver() override;
    void writeBuffer() override;
    void onSimulateReceived(const std::string& topic, const std::string& option, const std::string& content) override;

    void getWeatherFigures();
    void readData();

    void onRepoStateChanged(common::Repository::State state);

    Signal<const std::list<std::pair<std::string, WeatherInfo*>>&> onWeatherDataChanged;

protected:
    WeatherServer* mMonitor {nullptr};
    common::Repository mRepository;
    milliseconds mSyncTimer {milliseconds(300000)};
    bool isCheck = false;
    std::mutex mMutex;

private:
    WeatherDriver();
};


}

#endif //WEATHERDRIVER_H
