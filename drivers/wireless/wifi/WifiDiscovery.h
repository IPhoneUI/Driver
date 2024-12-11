#ifndef WIFIDISCOVERY_H
#define WIFIDISCOVERY_H

#include "WifiDeploy.h"
#include <wifi/WifiServiceDef.h>
#include <wifi/WifiProvider.h>
#include "WifiDriver.h"

namespace driver {

class WifiDiscovery {
public: 
    static WifiDiscovery& instance();
    virtual ~WifiDiscovery();

    void execute(milliseconds delta);

    void startDiscovery();
    void stopDiscovery();
    void requestConnectDevice(const std::string&);

private:
    WifiDiscovery();
    void handleDiscovering(milliseconds delta);
    void handleConnectDevice(milliseconds delta);

    base::shm::WifiDiscoveryDeviceShmem mDevice;
    bool mDiscoveryFlag {false};
    bool mConnectDeviceFlag {false};
    std::string mDiscoryDeviceAddr {""};
    int mStep {0};
    milliseconds mTime {milliseconds(0)};
    milliseconds mDiscoveringTime {milliseconds(0)};
    service::WifiAuthenDeviceStatus mAuthenStatus {service::WifiAuthenDeviceStatus::Fail};

    WifiDeploy* mWifiDeploy;
    base::shm::WifiProvider* mWifiProvider;
};

}


#endif // WIFIDISCOVERY_H
