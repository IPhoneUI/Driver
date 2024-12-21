#include "WifiDriver.h"
#include <utils/Logger.h>

namespace driver {

static WifiDriver* gInstance = 0;

WifiDriver::WifiDriver()
    : mConnectedDevice(new service::WifiDeviceInfo())
{
    mWifiPairing = new WifiPairing(this);
    mWifiDiscovery = new WifiDiscovery(this);

    common::DataRepoManager& dataRepo = common::DataRepoManager::instance();
    auto wifiRepo = dataRepo.addRepository("wifi");
    if (wifiRepo != nullptr)
    {
        wifiRepo->addParameter("data", common::ParameterIndex::Wifi_Data);
        wifiRepo->addParameter("wifistatus", common::ParameterIndex::Wifi_Status);

        Connection::connect(wifiRepo->onRepoStateChanged, std::bind(&WifiDriver::onRepoStateChanged, this, std::placeholders::_1));
    }

    common::DriverExecution::instance().addDriver("WifiDriver", this);
}

WifiDriver* WifiDriver::getInstance()
{
    if (gInstance == nullptr)
    {
        throw std::runtime_error("The WifiDriver has not been initialized yet");
    }

    return gInstance;
}

void WifiDriver::initialize()
{
    if (gInstance == nullptr)
    {
        gInstance = new WifiDriver();
    }
}

void WifiDriver::execute(milliseconds delta)
{
    mWifiPairing->execute(delta);
    mWifiDiscovery->execute(delta);
}

void WifiDriver::connectDriver()
{
    mIsReady = true;
    onDriverReady.emit();
}

void WifiDriver::onRepoStateChanged(common::Repository::State state)
{
    if (state == common::Repository::PullCompleted)
    {
        mWifiPairing->readData();
        mWifiDiscovery->readData();
    
        common::DataRepoManager& dataRepo = common::DataRepoManager::instance();
        common::Repository& repo = dataRepo.repository("wifi");

        utils::VariantList dataList = repo[common::ParameterIndex::Wifi_Data];
        {
            std::unordered_map<std::string, utils::Variant> item = (*dataList.begin());
            mConnectedDevice->password = item["password"];
            mConnectedDevice->autoconnectstatus = item["autoconnect"];
            mConnectedDevice->deviceinfo.name = item["name"];
            mConnectedDevice->deviceinfo.address = item["address"];
            mConnectedDevice->deviceinfo.privateAddr = item["privateaddress"];
            mConnectedDevice->deviceinfo.speedmode = static_cast<service::WifiSpeedMode>(int(item["wifisignal"]));
        }

        mWifiStatus = repo[common::ParameterIndex::Wifi_Status];
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