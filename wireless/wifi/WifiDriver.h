#ifndef WIFIDRIVER_H
#define WIFIDRIVER_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseDriver.h>
#include <wifi/WifiProvider.h>
#include "WifiDeploy.h"


namespace driver {

class WifiDriver : public common::BaseDriver 
{
public:
    explicit WifiDriver();

    void onMsqReceived() override;
    void initialize() override;
    void finialize() override;
    void execute(milliseconds delta) override;

    void registerClient(service::Msq_Client clientId, const std::string& clientName);

    void responseChangeWifiStatus(bool status);

    // std::vector<WifiDeviceInfo> getPairedDeviceList();
    // WifiDeviceInfo getPairedDevice(const std::string&);
    // bool addPairedDevice(WifiDeviceInfo device);
    // bool removePairedDevice(const std::string& addr);

    // void setConnectedDevice(WifiDeviceInfo device);
    // WifiDeviceInfo getConnectedDevice();

    // std::vector<WifiDiscoveryDeviceInfo> getDiscoveryDeviceList();
    // WifiDiscoveryDeviceInfo getDiscoveryDevice(const std::string&);
    // bool addDiscoveryDevice(WifiDiscoveryDeviceInfo device);
    // bool removeDiscoveryDevice(const std::string& addr);
    // WifiDeviceInfo queryWifiDevice(const std::string& addr);
    // std::vector<WifiDeviceInfo> getDiscoveryDeviceListPrivate();

private:
    std::shared_mutex mMutex;
    // std::vector<WifiDeviceInfo> mWifiDeviceData;
    // std::vector<WifiDeviceInfo> mPairedDevices;
    // std::vector<WifiDiscoveryDeviceInfo> mDiscoveryDevices;
    // WifiDeviceInfo mConnectedDevice;
    // std::vector<WifiDeviceInfo> mDiscoveryDevicePrivate;

    WifiProvider* mProvider;
    WifiDeploy* mDeploy;
};

}

#endif //WIFIDRIVER_H