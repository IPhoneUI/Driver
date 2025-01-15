#ifndef WEATHERDRIVER_H
#define WEATHERDRIVER_H

#include <string>
#include <thread>
#include <functional>
#include "WeatherServer.h"
#include <common/BaseDriver.h>

namespace driver {

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

    Signal<const std::vector<driver::WeatherInfo>&> onWeatherChanged;

protected:
    WeatherServer* mMonitor {nullptr};
    common::Repository mRepository;
    milliseconds mTimer;
    bool isCheck = false;

private:
    WeatherDriver();
};

}

#endif //WEATHERDRIVER_H
