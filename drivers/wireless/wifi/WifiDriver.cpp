#include "WifiDriver.h"
#include <utils/Logger.h>
#include "WifiDiscovery.h"
#include "WifiPairing.h"

namespace driver {

static WifiDriver* gInstance = 0;

WifiDriver::WifiDriver()
{
    mProvider = base::shm::WifiProvider::instance();
}

void WifiDriver::execute(milliseconds delta)
{
    // WifiDiscovery::instance().execute(delta);
    // WifiPairing::instance().execute(delta);
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
// void WifiDriver::requestChangeWifiStatus(bool status)
// {
//     auto result = mProvider->setWifiStatus(status);
//     if (result == base::shm::DataSetResult_Valid)
//     {
//         mDeploy->responseChangeWifiStatus(status);
//     }
// }

// void WifiDriver::requestConnectDevice(const std::string& address)
// {
//     LOG_INFO("Request connect device with address: %s", address.c_str());
//     for (const auto& item : base::shm::WifiProvider::instance()->getPairedDeviceList())
//     {
//         if (item.deviceinfo.address == address)
//         {
//             WifiPairing::instance().requestConnectDevice(address);
//             return;
//         }
//     }
//     for (const auto& item : base::shm::WifiProvider::instance()->getPairedDeviceList())
//     {
//         if (item.deviceinfo.address == address)
//         {
//             WifiDiscovery::instance().requestConnectDevice(address);
//             return;
//         }
//     }
// }

// void WifiDriver::requestCheckDevicePassword(const std::string& address, const std::string& password)
// {
//     std::function<bool(std::string, std::string, std::list<base::shm::WifiDeviceShmem>)> checkPassword = [](std::string address, std::string password, std::list<base::shm::WifiDeviceShmem> discoveryList) -> bool 
//     {
//         for (auto it = discoveryList.begin(); it != discoveryList.end(); it++) 
//         {
//             if (std::strcmp((*it).deviceinfo.address, address.c_str()) && std::strcmp((*it).password, password.c_str())) 
//             {
//                 return true;
//             }
//         }
//         return false;
//     };

//     bool result = false;
//     std::list<base::shm::WifiDeviceShmem> list = base::shm::WifiProvider::instance()->getPairedDeviceList();
//     if (checkPassword(address, password, list)) 
//     {
//         result = true;
//     }

//     mDeploy->responseCheckDevicePassword(result);
// }

}