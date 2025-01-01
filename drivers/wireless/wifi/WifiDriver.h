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
    friend class WifiPairing;
    friend class WifiDiscovery;
public:
    static WifiDriver* getInstance();
    static void initialize();
    
    void execute(milliseconds delta) override;
    void connectDriver() override;
    void writeBuffer() override;

    bool getWifiStatus() const
    {
        return mWifiStatus;
    }

    std::list<service::WifiDeviceInfo*> getPairedDeviceList() const
    {
        return mWifiPairing->getPairedDeviceList();
    }

    std::list<service::WifiDeviceInfo*> getDiscoveryDeviceList() const {
        return mWifiDiscovery->getWifiDiscoveryList();
    }

    service::WifiDeviceInfo* getConnectedDevice() const
    {
        return mConnectedDevice;
    }

    void startDiscovery();
    void requestChangeWifiStatus(bool status);
    void requestCheckDevicePassword(const std::string& address, const std::string& password);
    void requestConnectDevice(const std::string& address);
    void requestForgetDevice(const std::string& address);

    Signal<bool> onWifiStatusUpdated;
    Signal<service::WifiDeviceInfo*> onConnectedDeviceUpdated;
    Signal<const std::list<service::WifiDeviceInfo*>&> onPairedDeviceListUpdated;
    Signal<service::WifiDeviceInfo*> onAddDiscoryDeviceInfo;
    Signal<service::WifiDeviceInfo*> onRemoveDiscoryDeviceInfo;
    Signal<bool> onCheckPasswordStateUpdated;
    Signal<const service::WifiAuthenDeviceStatus&> onWifiAuthenDeviceStatusUpdated;

private: 
    void onRepoStateChanged(common::Repository::State state);

private:
    explicit WifiDriver();
    common::Repository mRepository;
    std::shared_mutex mMutex;
    bool mWifiStatus;
    service::WifiDeviceInfo* mConnectedDevice;
    WifiPairing* mWifiPairing;
    WifiDiscovery* mWifiDiscovery;
};

}

#endif //WIFIDRIVER_H
