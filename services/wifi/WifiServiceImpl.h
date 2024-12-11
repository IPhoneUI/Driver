#ifndef WIFISERVICEIMPL_H
#define WIFISERVICEIMPL_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseServiceImpl.h>
#include <wifi/WifiProvider.h>
#include "WifiServiceDeploy.h"

namespace service {

class WifiServiceImpl : public common::BaseServiceImpl 
{
public:
    explicit WifiServiceImpl();

    void onMsqReceived() override;
    void initialize() override;
    void finialize() override;
    void execute(milliseconds delta) override;

    void registerClient(const std::string& clientName);
    void requestChangeWifiStatus(bool status);
    void requestCheckDevicePassword(const std::string& address, const std::string& password);
    void requestConnectDevice(const std::string& address);

private:
    WifiServiceDeploy* mDeploy;
    base::shm::WifiProvider* mProvider;
};

}

#endif //WIFISERVICEIMPL_H