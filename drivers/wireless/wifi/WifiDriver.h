#ifndef WIFIDRIVER_H
#define WIFIDRIVER_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseDriver.h>
#include <wifi/WifiServiceDef.h>
#include <vector>
#include "WifiDiscovery.h"
#include "WifiPairing.h"

namespace driver {

class WifiDriver : public common::BaseDriver 
{
public:
    static WifiDriver* getInstance();
    static void initialize();
    
    void execute(milliseconds delta) override;
    void connectDriver() override;
    void readDataFromDatabase() override;

    bool getWifiStatus() const
    {
        return mWifiStatus;
    }

    std::list<service::WifiDeviceInfo*> getPairedDeviceList() const
    {
        return mWifiPairing->getPairedDeviceList();
    }

    service::WifiDeviceInfo* getConnectedDevice() const
    {
        return mConnectedDevice;
    }

    void startDiscovery();
    void requestChangeWifiStatus(bool status);
    void requestCheckDevicePassword(const std::string& address, const std::string& password);
    void requestConnectDevice(const std::string& address);

    Signal<bool> onWifiStatusUpdated;
    Signal<service::WifiDeviceInfo*> onConnectedDeviceUpdated;
    Signal<const std::list<service::WifiDeviceInfo*>&> onPairedDeviceListUpdated;
    Signal<service::WifiDiscoveryDeviceInfo*> onAddDiscoryDeviceInfo;

private:
    explicit WifiDriver();

    std::shared_mutex mMutex;
    bool mWifiStatus;
    service::WifiDeviceInfo* mConnectedDevice;
    WifiPairing* mWifiPairing;
    WifiDiscovery* mWifiDiscovery;
};

}

#endif //WIFIDRIVER_H