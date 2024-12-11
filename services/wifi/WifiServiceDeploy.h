#ifndef WIFISERVICEDEPLOY_H
#define WIFISERVICEDEPLOY_H

#include <string>
#include <thread>
#include <list>
#include <functional>
#include <mutex>
#include <shared_mutex>
#include <common/BaseDeploy.h>
#include <wifi/WifiProvider.h>

namespace service {

class WifiServiceDeploy : public common::BaseDeploy
{
public:
    static WifiServiceDeploy* instance();

    void responseDriverReady(const std::string& clientName);
    void responseSync(const std::string& clientName);
    void responseChangeWifiStatus(bool status);
    void responseDiscoveryDeviceUpdated(const base::shm::WifiDiscoveryDeviceShmem& device);
    void responseCheckDevicePassword(bool result);

private:
    explicit WifiServiceDeploy();
    ~WifiServiceDeploy();

    std::mutex mMutex;
    base::shm::WifiProvider* mProvider; 
};

}

#endif //WIFISERVICEDEPLOY_H
