#ifndef WIFISERVICEIMPL_H
#define WIFISERVICEIMPL_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseServiceImpl.h>
#include <wifi/WifiProvider.h>
#include <wifi/WifiDriver.h>
#include "WifiServiceDeploy.h"

namespace service {

class WifiServiceImpl : public common::BaseServiceImpl 
{
public:
    explicit WifiServiceImpl();

    void onMsqReceived() override;
    void initialize() override;
    void finialize() override;

    void requestSync();

public:
    void onWifiDriverReady();
    void onWifiStatusUpdated(bool);
    void onPairedDeviceListUpdated(const std::list<service::WifiDeviceInfo*>&);
    void onRemoveDiscoryDeviceInfo(service::WifiDeviceInfo* device);
    void onConnectedDeviceUpdated(service::WifiDeviceInfo* device);
    void onAddDiscoryDeviceInfo(service::WifiDeviceInfo* device);
    void onCheckPasswordStateUpdated(bool);

private:
    WifiServiceDeploy* mDeploy;
    driver::WifiDriver* mWifiDriver;
    base::shm::WifiProvider* mProvider;
};

}

#endif //WIFISERVICEIMPL_H
