#ifndef WIFISERVICEDEPLOY_H
#define WIFISERVICEDEPLOY_H

#include <string>
#include <thread>
#include <list>
#include <functional>
#include <mutex>
#include <shared_mutex>
#include <common/BaseDeploy.h>
#include <wifi/WifiServiceDef.h>

namespace service {

class WifiServiceDeploy : public common::BaseDeploy
{
public:
    static WifiServiceDeploy* instance();

    void responseServiceReady(const std::string& clientName);
    void responseChangeWifiStatus(bool status);
    void responseDiscoveryDeviceUpdated(service::WifiDiscoveryDeviceInfo* device);
    void responseCheckDevicePassword(bool result);
    void responseConnectedDeviceUpdated();
    void responsePairedListUpdated();

private:
    explicit WifiServiceDeploy();
    ~WifiServiceDeploy();

    std::mutex mMutex;
};

}

#endif //WIFISERVICEDEPLOY_H
