#ifndef WIFIDISCOVERY_H
#define WIFIDISCOVERY_H

#include <wifi/WifiServiceDef.h>
#include <common/BaseDriver.h>

namespace driver {

class WifiDriver;
class WifiDiscovery {
public: 
    WifiDiscovery(WifiDriver *driver);

    void execute(milliseconds delta);
    void readData();
    void startDiscovery();
    void stopDiscovery();
    void requestConnectDevice(const std::string&);

private:
    void handleDiscovering(milliseconds delta);
    void handleConnectDevice(milliseconds delta);

    WifiDriver* mWifiDriver;
    std::list<service::WifiDeviceInfo*> mDiscoryDeviceList;
    service::WifiDeviceInfo* mDevice;
    bool mDiscoveryFlag {false};
    bool mConnectDeviceFlag {false};
    std::string mDiscoryDeviceAddr {""};
    int mStep {0};
    milliseconds mTime {milliseconds(0)};
    milliseconds mDiscoveringTime {milliseconds(0)};
    service::WifiAuthenDeviceStatus mAuthenStatus {service::WifiAuthenDeviceStatus::Fail};
};

}


#endif // WIFIDISCOVERY_H
