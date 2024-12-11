#ifndef WIFIDRIVER_H
#define WIFIDRIVER_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseDriver.h>
#include <wifi/WifiProvider.h>
#include <vector>

namespace driver {

class WifiDriver : public common::BaseDriver 
{
public:
    static WifiDriver* getInstance();
    static void initialize();
    
    void execute(milliseconds delta) override;
    void connectDriver() override;

    // void registerClient(base::msq::Msq_Client clientId, const std::string& clientName);

    // void requestChangeWifiStatus(bool status);
    // void requestCheckDevicePassword(const std::string& address, const std::string& password);
    // void requestConnectDevice(const std::string& address);

private:
    explicit WifiDriver();

    std::shared_mutex mMutex;

    base::shm::WifiProvider* mProvider;
};

}

#endif //WIFIDRIVER_H