#include "WifiDriver.h"
#include <utils/Logger.h>
#include <functional>

namespace driver {

static WifiDriver* gInstance = 0;

WifiDriver::WifiDriver()
    : mConnectedDevice(new service::WifiDeviceInfo())
{
    mRepository.setName("wifi");

    mWifiPairing = new WifiPairing(this);
    mWifiDiscovery = new WifiDiscovery(this);

    mRepo.addParam("connected", common::ParameterIndex::Wifi_Connected);
    mRepo.addParam("discovery", common::ParameterIndex::Wifi_Discovery);
    mRepo.addParam("paired", common::ParameterIndex::Wifi_Paired);
    mRepo.addParam("wifistatus", common::ParameterIndex::Wifi_Status);

    Connection::connect(mRepository.onRepoStateChanged, std::bind(&WifiDriver::onRepoStateChanged, this, std::placeholders::_1));

    mRepo.pull();

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

void WifiDriver::writeBuffer()
{
    mRepository[common::ParameterIndex::Wifi_Status] = mWifiStatus;

    // utils::VariantList wifiList;
    // std::unordered_map<std::string, utils::Variant> item;
    // item["password"] = mConnectedDevice->password;
    // item["autoconnect"] = mConnectedDevice->autoconnectstatus;
    // item["name"] = mConnectedDevice->deviceinfo.name;
    // item["address"] = mConnectedDevice->deviceinfo.address;
    // item["privateaddress"] = mConnectedDevice->deviceinfo.privateAddr;
    // item["wifisignal"] = static_cast<int>(mConnectedDevice->deviceinfo.speedmode);

    // wifiList.push(item);
    mRepository.push();
}

void WifiDriver::readData() {

    LOG_INFO("THAIVD --- READ DATA");
    utils::VariantList dataList = mRepository[common::ParameterIndex::Wifi_Data];
    for (int it = 0; it < dataList.size(); it++) {

        std::unordered_map<std::string, utils::Variant> item = *(std::next(dataList.begin(), it));
        service::WifiDeviceType typeDevice = static_cast<service::WifiDeviceType>(int(item["type"]));

        service::WifiDeviceInfo* wifiItem = new service::WifiDeviceInfo(
            item["name"], 
            item["address"], 
            static_cast<service::WifiDeviceType>(int(item["type"])),
            item["privateaddress"],
            static_cast<service::WifiSpeedMode>(int(item["wifisignal"])),
            item["password"],
            item["autoconnect"]
        );

        if (wifiItem == nullptr) {
            continue;
        }

        std::string name = item["name"];
        std::string address = item["address"];
        int type = int(item["type"]);
        
        LOG_INFO("THAIVD -- INFO DEVICE => name: %s addr: %s type: %d", name.c_str(), address.c_str(), type);
        if (typeDevice == service::WifiDeviceType::Connected) {
            mConnectedDevice = wifiItem;
            mWifiPairing->mPairedDeviceList.emplace_back(mConnectedDevice);
        } else if (typeDevice == service::WifiDeviceType::Paired) {
            mWifiPairing->mPairedDeviceList.emplace_back(wifiItem);
        } else {
            mWifiDiscovery->mDiscoryDeviceList.emplace_back(wifiItem);
        }
    }
}

void WifiDriver::onRepoStateChanged(common::Repository::State state)
{
    if (state == common::Repository::PullCompleted)
    {
        mWifiPairing->readData();
        mWifiDiscovery->readData();
    
        utils::VariantObj dataList = mRepo[common::ParameterIndex::Wifi_Connected];
        {
            std::unordered_map<std::string, utils::Variant> item = dataList;
            mConnectedDevice->password = item["password"];
            mConnectedDevice->autoconnectstatus = item["autoconnect"];
            mConnectedDevice->name = item["name"];
            mConnectedDevice->address = item["address"];
            mConnectedDevice->privateAddr = item["privateaddress"];
            mConnectedDevice->speedmode = static_cast<service::WifiSpeedMode>(int(item["wifisignal"]));
        }

        mWifiStatus = mRepo[common::ParameterIndex::Wifi_Status];
    }
}

std::list<service::WifiDeviceInfo*> WifiDriver::getPairedDeviceList() const
{
    return mWifiPairing->mPairedDeviceList;
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
    // std::string addressTemp = address;
    // addressTemp.erase(addressTemp.end() - 1);

    // service::WifiDeviceInfo* pairedDevice = mWifiPairing->getPairedDeviceInfo(address);
    // if (pairedDevice != nullptr) {
    //     mWifiPairing->requestConnectDevice(pairedDevice);
    //     return;
    // }

    // service::WifiDeviceInfo* discoveryDevice = mWifiDiscovery->getDiscoveryDeviceInfo(address);
    // if (discoveryDevice != nullptr) {
    //     mWifiDiscovery->setWaitPairDevice(discoveryDevice);
    //     onRequestAuthencatePassword.emit(discoveryDevice->address);
    // }
}

void WifiDriver::requestCheckDevicePassword(const std::string& address, const std::string& password)
{
    // service::WifiDeviceInfo* discoveryDevice = mWifiDiscovery->getDiscoveryDeviceInfo(address);
    // if (!address.empty() && (discoveryDevice != nullptr)) {
    //     mWifiDiscovery->requestConnectDevice(discoveryDevice, password);
    // }
}

void WifiDriver::setConnectedDevice(service::WifiDeviceInfo* device) {
    mConnectedDevice = device;
}

}
