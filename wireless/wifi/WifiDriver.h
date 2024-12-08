#ifndef WIFIDRIVER_H
#define WIFIDRIVER_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseDriver.h>
#include <wifi/WifiProvider.h>
#include <vector>
#include "WifiDeploy.h"

namespace driver {

class WifiDriver : public common::BaseDriver 
{
public:
    explicit WifiDriver();

    void onMsqReceived() override;
    void initialize() override;
    void finialize() override;
    void execute(milliseconds delta) override;

    void registerClient(service::Msq_Client clientId, const std::string& clientName);

    void requestChangeWifiStatus(bool status);
    void requestCheckDevicePassword(const std::string& address, const std::string& password);
    void requestConnectDevice(const std::string& address);

private:
    std::shared_mutex mMutex;

    WifiProvider* mProvider;
    WifiDeploy* mDeploy;
};

}

#endif //WIFIDRIVER_H