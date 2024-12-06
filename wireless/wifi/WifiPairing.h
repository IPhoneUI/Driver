#ifndef WIFIPAIRING_H
#define WIFIPAIRING_H

#include "WifiDeploy.h"
#include <wifi/WifiServiceDef.h>
#include <wifi/WifiProvider.h>
#include "WifiDriver.h"

namespace driver {

class WifiPairing {
public: 
    static WifiPairing& instance();
    virtual ~WifiPairing();

    void execute(milliseconds delta);
    std::list<WifiDeviceShmem> getPairedDeviceList();
    void connectDevice(const std::string& addr);
    void resetConnectDeviceFlag();

private:
    WifiPairing();

    std::string mPairingAddr {""};
    bool mPairingFlag {false};
    int mStep {0};
    milliseconds mTime {milliseconds(0)};
    service::WifiAuthenDeviceStatus mAuthenStatus {service::WifiAuthenDeviceStatus::Fail};

    WifiProvider* mWifiProvider;
    WifiDeploy* mWifiDeploy;
};

}


#endif // WIFIPAIRING_H
