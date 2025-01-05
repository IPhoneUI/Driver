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
    void removePairedDevice(const std::string& addr);
    void cancelConnecting();

    void readData();
    void writeBuffer();

    void appendNewPairedDevice(service::WifiDeviceInfo* newDevice);

private:
    WifiDriver* mWifiDriver;
    std::string mPairingAddr {""};
    bool mPairingFlag {false};
    int mStep {0};
    milliseconds mTime {milliseconds(0)};
    service::WifiAuthenDeviceStatus mAuthenStatus {service::WifiAuthenDeviceStatus::Fail};
    std::list<service::WifiDeviceInfo*> mPairedDevices;
    service::WifiDeviceInfo* mPairingDevice;
};

}


#endif // WIFIPAIRING_H
