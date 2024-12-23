#ifndef WIFIPAIRING_H
#define WIFIPAIRING_H

#include <wifi/WifiServiceDef.h>
#include <common/BaseDriver.h>

namespace driver {

class WifiDriver;
class WifiPairing {
public: 
    WifiPairing(WifiDriver* driver);

    void execute(milliseconds delta);
    std::list<service::WifiDeviceInfo*> getPairedDeviceList();
    void requestConnectDevice(service::WifiDeviceInfo* device);
    service::WifiDeviceInfo* getPairedDeviceInfo(const std::string& address) const;

    void readData();

private:
    WifiDriver* mWifiDriver;
    std::string mPairingAddr {""};
    bool mPairingFlag {false};
    int mStep {0};
    milliseconds mTime {milliseconds(0)};
    service::WifiAuthenDeviceStatus mAuthenStatus {service::WifiAuthenDeviceStatus::None};
    std::list<service::WifiDeviceInfo*> mPairedDeviceList;
    service::WifiDeviceInfo* mPairingDevice;
};

}


#endif // WIFIPAIRING_H
