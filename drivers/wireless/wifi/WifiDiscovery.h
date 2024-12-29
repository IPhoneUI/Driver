#ifndef WIFIDISCOVERY_H
#define WIFIDISCOVERY_H

#include <wifi/WifiServiceDef.h>
#include <common/BaseDriver.h>

namespace driver {

class WifiDriver;
class WifiDiscovery {
    friend class WifiDriver;
public: 
    WifiDiscovery(WifiDriver *driver = nullptr);
    ~WifiDiscovery();

    void execute(milliseconds delta);
    void readData();
    void startDiscovery();
    void stopDiscovery();
    void requestConnectDevice(service::WifiDeviceInfo* device, const std::string& password);
    void setWaitPairDevice(service::WifiDeviceInfo* device);

    friend std::ostream& operator<<(std::ostream& os, service::WifiDeviceType type);

private:
    void handleDiscovering(milliseconds delta);
    void handleConnectDevice(milliseconds delta);

    WifiDriver* mWifiDriver;
    std::list<service::WifiDeviceInfo*> mDiscoryDeviceList;
    service::WifiDeviceInfo* mDevice;
    bool mDiscoveryFlag {false};
    bool mConnectDeviceFlag {false};
    std::string mDiscoryDeviceAddr {""};
    std::string mPasswordDevice {""};
    int mStep {0};
    milliseconds mTime {milliseconds(0)};
    milliseconds mDiscoveringTime {milliseconds(0)};
    service::WifiAuthenDeviceStatus mAuthenStatus {service::WifiAuthenDeviceStatus::Fail};
};

std::ostream& operator<<(std::ostream& os, service::WifiDeviceType type) {
    if (type == service::WifiDeviceType::Connected) {
        os << "Connected";
    } else if (type == service::WifiDeviceType::Paired) {
        os << "Paird";
    } else {
        os << "Discovery";
    }
    return os;
}

}


#endif // WIFIDISCOVERY_H
