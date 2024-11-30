#include "WifiDriver.h"
#include <utils/Logger.h>

namespace driver {

WifiDriver::WifiDriver()
{

    // storages::DataRepoManager& dataRepo = storages::DataRepoManager::instance();

    // if (dataRepo.isReady())
    // {
    //     storages::Repository& repo = dataRepo.getRepository("Wifi");

    //     auto dataMap = repo[storages::ParameterIndex::Wifi_Data].toList();

    //     for (auto it = dataMap.begin(); it != dataMap.end(); ++it)
    //     {
    //         std::unordered_map<std::string, storages::Parameter> item = (*it);
    //         WifiDiscoveryDeviceInfo discoryInfo;
    //         discoryInfo.name = std::string(item["name"]);
    //         discoryInfo.address = std::string(item["address"]);
    //         discoryInfo.privateAddr = bool(item["privateaddress"]);
    //         WifiSpeedMode speedMode = static_cast<WifiSpeedMode>(int(item["wifisignal"]));

    //         WifiDeviceInfo deviceInfo;
    //         deviceInfo.deviceInfo = discoryInfo;
    //         deviceInfo.password = std::string(item["password"]);
    //         deviceInfo.autoConnectStatus = bool(item["autoconnect"]);

    //         mWifiDeviceData.push_back(deviceInfo);
    //     }
    // }

    // for (int i = 1; i < 6; ++i)
    // {
    //     auto device = mWifiDeviceData[i];
    //     mPairedDevices.push_back(device);
    // }

    // for (int i = 6; i < 14; ++i)
    // {
    //     auto device = mWifiDeviceData[i];
    //     mDiscoveryDevicePrivate.push_back(device);
    //     mDiscoveryDevices.push_back(device.deviceInfo);
    // }

    // mConnectedDevice = mWifiDeviceData[0];
}

void WifiDriver::onMsqReceived()
{
}

void WifiDriver::initialize()
{
    LOG_INFO("WifiDriver initialize");
}

void WifiDriver::finialize()
{
    LOG_INFO("WifiDriver finialize");
}

std::vector<WifiDeviceInfo> WifiDriver::getPairedDeviceList()
{
    std::shared_lock<std::shared_mutex> lock(mMutex);
    return mPairedDevices;
}

WifiDeviceInfo WifiDriver::getPairedDevice(const std::string& addr)
{
    for (auto& item : mPairedDevices)
    {
        if (item.deviceInfo.address == addr)
            return item;
    }
    return {};
}

bool WifiDriver::addPairedDevice(WifiDeviceInfo device)
{
    std::unique_lock<std::shared_mutex> lock(mMutex);

    bool flag = true;
    for (auto& item : mPairedDevices)
    {
        if (item.deviceInfo.address == device.deviceInfo.address)
            flag = false;
    }

    if (flag)
    {
        mPairedDevices.push_back(device);
        return true;
    }
    return false;
}

bool WifiDriver::removePairedDevice(const std::string& addr)
{
    std::unique_lock<std::shared_mutex> lock(mMutex);

    for (int i = 0; i < mPairedDevices.size(); ++i)
    {
        if (mPairedDevices[i].deviceInfo.address == addr)
        {
            // Remove paired device
            mPairedDevices.erase(mPairedDevices.begin() + i);
            return true;
        }
    }
    return false;
}

void WifiDriver::setConnectedDevice(WifiDeviceInfo device)
{
    mConnectedDevice = device;
}

WifiDeviceInfo WifiDriver::getConnectedDevice()
{
    return mConnectedDevice;
}

std::vector<WifiDiscoveryDeviceInfo> WifiDriver::getDiscoveryDeviceList()
{
    return mDiscoveryDevices;
}

WifiDiscoveryDeviceInfo WifiDriver::getDiscoveryDevice(const std::string& addr)
{
    for (auto& item : mDiscoveryDevices)
    {
        if (item.address == addr)
            return item;
    }
    return {};
}

bool WifiDriver::addDiscoveryDevice(WifiDiscoveryDeviceInfo device)
{
    std::unique_lock<std::shared_mutex> lock(mMutex);

    bool flag = true;
    for (auto& item : mDiscoveryDevices)
    {
        if (item.address == device.address)
            flag = false;
    }

    if (flag)
    {
        mDiscoveryDevices.push_back(device);
        return true;
    }
    return false;
}

bool WifiDriver::removeDiscoveryDevice(const std::string& addr)
{
    std::unique_lock<std::shared_mutex> lock(mMutex);

    for (int i = 0; i < mDiscoveryDevices.size(); ++i)
    {
        // Remove discovery device
        mDiscoveryDevices.erase(mDiscoveryDevices.begin() + i);
        return true;
    }
    return false;
}

WifiDeviceInfo WifiDriver::queryWifiDevice(const std::string& addr)
{
    for (auto& device : mWifiDeviceData)
    {
        if (addr == device.deviceInfo.address)
        {
            return device;
        }
    }
    return {};
}

std::vector<WifiDeviceInfo> WifiDriver::getDiscoveryDeviceListPrivate() 
{
    return mDiscoveryDevicePrivate;
}

}