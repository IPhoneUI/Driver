#include "WifiDriver.h"
#include <utils/Logger.h>

namespace driver {

static WifiDriver* gInstance = 0;

WifiDriver::WifiDriver()
    : mConnectedDevice(new service::WifiDeviceInfo())
{
    mRepo.setName("wifi");
    mWifiPairing = new WifiPairing(this);
    mWifiDiscovery = new WifiDiscovery(this);

    mRepo.addParam("data", common::ParameterIndex::Wifi_Data);
    mRepo.addParam("wifistatus", common::ParameterIndex::Wifi_Status);

    Connection::connect(mRepo.onRepoStateChanged, std::bind(&WifiDriver::onRepoStateChanged, this, std::placeholders::_1));

    mRepo.pull();val:

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
    mRepo[common::ParameterIndex::Wifi_Status] = mWifiStatus;

    // utils::VariantList wifiList;
    // std::unordered_map<std::string, utils::Variant> item;
    // item["password"] = mConnectedDevice->password;
    // item["autoconnect"] = mConnectedDevice->autoconnectstatus;
    // item["name"] = mConnectedDevice->deviceinfo.name;
    // item["address"] = mConnectedDevice->deviceinfo.address;
    // item["privateaddress"] = mConnectedDevice->deviceinfo.privateAddr;
    // item["wifisignal"] = static_cast<int>(mConnectedDevice->deviceinfo.speedmode);

    // wifiList.push(item);
    mRepo.push();
}

void WifiDriver::onRepoStateChanged(common::Repository::State state)
{
    if (state == common::Repository::PullCompleted)
    {
        mWifiPairing->readData();
        mWifiDiscovery->readData();
    
        utils::VariantList dataList = mRepo[common::ParameterIndex::Wifi_Data];
        {
            std::unordered_map<std::string, utils::Variant> item = (*dataList.begin());
            mConnectedDevice->password = item["password"];
            mConnectedDevice->autoconnectstatus = item["autoconnect"];
            mConnectedDevice->deviceinfo.name = item["name"];
            mConnectedDevice->deviceinfo.address = item["address"];
            mConnectedDevice->deviceinfo.privateAddr = item["privateaddress"];
            mConnectedDevice->deviceinfo.speedmode = static_cast<service::WifiSpeedMode>(int(item["wifisignal"]));
        }

        mWifiStatus = mRepo[common::ParameterIndex::Wifi_Status];
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
