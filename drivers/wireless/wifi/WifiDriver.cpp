#include "WifiDriver.h"
#include <utils/Logger.h>
#include <common/DriverExecution.h>

namespace driver {

static WifiDriver* gInstance = 0;

WifiDriver::WifiDriver()
    : mConnectedDevice(new service::WifiDeviceInfo())
{
    common::DriverExecution::instance().addDriver("WifiDriver", this);
    mWifiPairing = new WifiPairing(this);
    mWifiDiscovery = new WifiDiscovery(this);
    readDataFromDatabase();
}

void WifiDriver::execute(milliseconds delta)
{
    mWifiPairing->execute(delta);
    mWifiDiscovery->execute(delta);
}

WifiDriver* WifiDriver::getInstance()
{
    if (gInstance == nullptr)
    {
        gInstance = new WifiDriver();
    }

    return gInstance;
}

void WifiDriver::initialize()
{
    LOG_INFO("WifiDriver initialize");
    if (gInstance == nullptr)
    {
        gInstance = new WifiDriver();
    }
}

void WifiDriver::connectDriver()
{
    mIsReady = true;
    onDriverReady.emit();
}

void WifiDriver::readDataFromDatabase()
{
    mWifiPairing->readData();
    mWifiDiscovery->readData();
    
    common::DataRepoManager& dataRepo = common::DataRepoManager::instance();

    if (dataRepo.isReady())
    {
        common::Repository& repo = dataRepo.getRepository("wifi");

        auto dataMap = repo[common::ParameterIndex::Wifi_Data].toList();
        {
            std::unordered_map<std::string, common::Parameter> item = (*dataMap.begin());
            mConnectedDevice->password = std::string(item["password"]);
            mConnectedDevice->autoconnectstatus = item["autoconnect"];
            mConnectedDevice->deviceinfo.name = std::string(item["name"]);
            mConnectedDevice->deviceinfo.address = std::string(item["address"]);
            mConnectedDevice->deviceinfo.privateAddr = item["privateaddress"];
            mConnectedDevice->deviceinfo.speedmode = static_cast<service::WifiSpeedMode>(int(item["wifisignal"]));
        }
    }
}

void WifiDriver::requestChangeWifiStatus(bool status)
{
    if (mWifiStatus == status)
        return;
    
    mWifiStatus = status;
    onWifiStatusUpdated.emit(mWifiStatus);
}

void WifiDriver::startDiscovery()
{
    mWifiDiscovery->startDiscovery();
}

void WifiDriver::requestConnectDevice(const std::string& address)
{
    // LOG_INFO("Request connect device with address: %s", address.c_str());
    // for (const auto& item : base::shm::WifiProvider::instance()->getPairedDeviceList())
    // {
    //     if (item.deviceinfo.address == address)
    //     {
    //         WifiPairing::instance().requestConnectDevice(address);
    //         return;
    //     }
    // }
    // for (const auto& item : base::shm::WifiProvider::instance()->getPairedDeviceList())
    // {
    //     if (item.deviceinfo.address == address)
    //     {
    //         WifiDiscovery::instance().requestConnectDevice(address);
    //         return;
    //     }
    // }
}

void WifiDriver::requestCheckDevicePassword(const std::string& address, const std::string& password)
{
    // std::function<bool(std::string, std::string, std::list<base::shm::WifiDeviceShmem>)> checkPassword = [](std::string address, std::string password, std::list<base::shm::WifiDeviceShmem> discoveryList) -> bool 
    // {
    //     for (auto it = discoveryList.begin(); it != discoveryList.end(); it++) 
    //     {
    //         if (std::strcmp((*it).deviceinfo.address, address.c_str()) && std::strcmp((*it).password, password.c_str())) 
    //         {
    //             return true;
    //         }
    //     }
    //     return false;
    // };

    // bool result = false;
    // std::list<base::shm::WifiDeviceShmem> list = base::shm::WifiProvider::instance()->getPairedDeviceList();
    // if (checkPassword(address, password, list)) 
    // {
    //     result = true;
    // }

    // mDeploy->responseCheckDevicePassword(result);
}

}