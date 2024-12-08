#ifndef WIFIDEPLOY_H
#define WIFIDEPLOY_H

#include <string>
#include <thread>
#include <list>
#include <functional>
#include <mutex>
#include <shared_mutex>
#include <common/BaseDeploy.h>
#include <wifi/WifiProvider.h>

namespace driver {

class WifiDeploy : public common::BaseDeploy
{
public:
    static WifiDeploy* instance();

    void responseDriverReady(const std::string& clientName);
    void responseSync(const std::string& clientName);
    void responseChangeWifiStatus(bool status);
    void responseDiscoveryDeviceUpdated(const base::shm::WifiDiscoveryDeviceShmem& device);
    void responseCheckDevicePassword(bool result);

private:
    explicit WifiDeploy();
    ~WifiDeploy();

    base::shm::WifiProvider* mProvider;
    std::mutex mMutex;
};

}

#endif //WIFIDEPLOY_H