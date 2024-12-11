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
    std::list<base::shm::WifiDeviceShmem> getPairedDeviceList();
    void requestConnectDevice(const std::string& addr);

private:
    WifiPairing();

    std::string mPairingAddr {""};
    bool mPairingFlag {false};
    int mStep {0};
    milliseconds mTime {milliseconds(0)};
    service::WifiAuthenDeviceStatus mAuthenStatus {service::WifiAuthenDeviceStatus::Fail};
    base::shm::WifiDeviceShmem* mPairingDevice;

    base::shm::WifiProvider* mWifiProvider;
    WifiDeploy* mWifiDeploy;
};

}


#endif // WIFIPAIRING_H
