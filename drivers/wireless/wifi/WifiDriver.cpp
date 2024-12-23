#include "WifiDriver.h"
#include <utils/Logger.h>

namespace driver {

static WifiDriver* gInstance = 0;

WifiDriver::WifiDriver()
    : mConnectedDevice(new service::WifiDeviceInfo())
{
    mWifiPairing = new WifiPairing(this);
    mWifiDiscovery = new WifiDiscovery(this);
    common::DriverExecution::instance().addDriver("WifiDriver", this);
}

WifiDriver* WifiDriver::getInstance()
{
    if (gInstance == nullptr)
    {
        gInstance = new WifiDriver();
    }

    return gInstance;
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

std::list<service::WifiDeviceInfo *> WifiDriver::getPairedDeviceList() const
{
    return mWifiPairing->getPairedDeviceList();
}

std::list<service::WifiDiscoveryDeviceInfo *> WifiDriver::getDiscoveryDeviceList() const
{
    return mWifiDiscovery->getDiscoveryDeviceList();
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
    std::string addressTemp = address;
    addressTemp.erase(addressTemp.end() - 1);

    service::WifiDeviceInfo* pairedDevice = mWifiPairing->getPairedDeviceInfo(address);
    if (pairedDevice != nullptr) {
        mWifiPairing->requestConnectDevice(pairedDevice);
        return;
    }

    service::WifiDiscoveryDeviceInfo* discoveryDevice = mWifiDiscovery->getDiscoveryDeviceInfo(address);
    if (discoveryDevice != nullptr) {
        mWifiDiscovery->setWaitPairDevice(discoveryDevice);
        onRequestAuthencatePassword.emit(discoveryDevice->address);
    }
}

void WifiDriver::requestCheckDevicePassword(const std::string& address, const std::string& password)
{
    service::WifiDiscoveryDeviceInfo* discoveryDevice = mWifiDiscovery->getDiscoveryDeviceInfo(address);
    if (!address.empty() && (discoveryDevice != nullptr)) {
        mWifiDiscovery->requestConnectDevice(discoveryDevice, password);
    }
}

void WifiDriver::setConnectedDevice(service::WifiDeviceInfo* device) {
    mConnectedDevice = device;
}

}
