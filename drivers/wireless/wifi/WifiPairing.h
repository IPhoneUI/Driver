#ifndef WIFIPAIRING_H
#define WIFIPAIRING_H

#include <wifi/WifiServiceDef.h>
#include <common/BaseDriver.h>

namespace driver {

class WifiDriver;
class WifiPairing {
    friend class WifiDriver;
public: 
    WifiPairing(WifiDriver* driver);

    void execute(milliseconds delta);
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
