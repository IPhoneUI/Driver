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
    void startDiscovery();
    void stopDiscovery();
    void requestConnectDevice(const std::string&);
    void requestCheckPassword(const std::string&, const std::string&);

    std::list<service::WifiDeviceInfo*> getWifiDiscoveryList() const {
        return mDiscoveryDevices;
    }

    void readData();
    void writeBuffer();

private:
    void handleDiscovering(milliseconds delta);
    void handleConnectDevice(milliseconds delta);

    WifiDriver* mWifiDriver;
    std::list<service::WifiDeviceInfo*> mDiscoveryDevices;
    service::WifiDeviceInfo* mDevice;
    service::WifiDeviceInfo* mPairingDevice;
    bool mDiscoveryFlag {false};
    bool mConnectDeviceFlag {false};
    std::string mDiscoryDeviceAddr {""};
    std::string mPassword {""};
    int mStep {0};
    milliseconds mTime {milliseconds(0)};
    milliseconds mDiscoveringTime {milliseconds(0)};
    service::WifiAuthenDeviceStatus mAuthenStatus {service::WifiAuthenDeviceStatus::Fail};
};

}


#endif // WIFIDISCOVERY_H
