#ifndef WIFIDRIVER_H
#define WIFIDRIVER_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseServiceImpl.h>
#include <wifi/WifiProvider.h>
#include <vector>
#include "WifiDeploy.h"

namespace driver {

class WifiDriver : public common::BaseServiceImpl 
{
public:
    explicit WifiDriver();

    void onMsqReceived() override;
    void initialize() override;
    void finialize() override;
    void execute(milliseconds delta) override;

    void registerClient(base::msq::Msq_Client clientId, const std::string& clientName);

    void requestChangeWifiStatus(bool status);
    void requestCheckDevicePassword(const std::string& address, const std::string& password);
    void requestConnectDevice(const std::string& address);

private:
    std::shared_mutex mMutex;

    base::shm::WifiProvider* mProvider;
    WifiDeploy* mDeploy;
};

}

#endif //WIFIDRIVER_H