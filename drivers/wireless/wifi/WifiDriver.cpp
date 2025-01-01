#include "WifiDriver.h"
#include <utils/Logger.h>

namespace driver {

static WifiDriver* gInstance = 0;

WifiDriver::WifiDriver()
    : mConnectedDevice(new service::WifiDeviceInfo())
{
    mRepository.setName("wifi");
    mWifiPairing = new WifiPairing(this);
    mWifiDiscovery = new WifiDiscovery(this);

    mRepository.addParam("connected", common::ParameterIndex::Wifi_Connected);
    mRepository.addParam("discovery", common::ParameterIndex::Wifi_Discovery);
    mRepository.addParam("paired", common::ParameterIndex::Wifi_Paired);
    mRepository.addParam("wifistatus", common::ParameterIndex::Wifi_Status);

    Connection::connect(mRepository.onRepoStateChanged, std::bind(&WifiDriver::onRepoStateChanged, this, std::placeholders::_1));

    mRepository.pull();

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
    
    std::unordered_map<std::string, utils::Variant> item;
    item["password"] = mConnectedDevice->password ;
    item["autoconnect"] = mConnectedDevice->autoconnectstatus;
    item["name"] = mConnectedDevice->name;
    item["address"] = mConnectedDevice->address;
    item["privateaddress"] = mConnectedDevice->privateAddr;
    item["wifisignal"] = static_cast<int>(mConnectedDevice->speedmode);
    utils::VariantObj obj = item;
    mRepository[common::ParameterIndex::Wifi_Connected] = obj;

    mWifiPairing->writeBuffer();
    mWifiDiscovery->writeBuffer();

    mRepository.push();
}

void WifiDriver::onRepoStateChanged(common::Repository::State state)
{
    if (state == common::Repository::PullCompleted)
    {
        mWifiPairing->readData();
        mWifiDiscovery->readData();
    
        utils::VariantObj dataList = mRepository[common::ParameterIndex::Wifi_Connected];
        {
            std::unordered_map<std::string, utils::Variant> item = dataList;
            mConnectedDevice->password = item["password"];
            mConnectedDevice->autoconnectstatus = item["autoconnect"];
            mConnectedDevice->name = item["name"];
            mConnectedDevice->address = item["address"];
            mConnectedDevice->privateAddr = item["privateaddress"];
            mConnectedDevice->speedmode = static_cast<service::WifiSpeedMode>(int(item["wifisignal"]));
        }

        mWifiStatus = mRepository[common::ParameterIndex::Wifi_Status];
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
    LOG_INFO("Request connect device with address: %s", address.c_str());

    std::string connectingAddr = address;
    connectingAddr.erase(connectingAddr.end() - 1);

    std::list<service::WifiDeviceInfo*> pairedList = mWifiPairing->getPairedDeviceList();

    for (std::list<service::WifiDeviceInfo*>::iterator it = pairedList.begin(); it != pairedList.end(); it++) {
        if ((*it)->address == connectingAddr) {
            mWifiPairing->setPairingDevice(*it);
            mWifiPairing->requestConnectDevice(connectingAddr);
            return;
        }
    }

    std::list<service::WifiDeviceInfo*> discoveryList = mWifiDiscovery->getWifiDiscoveryList();

    for (std::list<service::WifiDeviceInfo*>::iterator it = discoveryList.begin(); it != discoveryList.end(); it++) {
        if ((*it)->address == connectingAddr) {
            mWifiDiscovery->requestConnectDevice(connectingAddr);
            return;
        }
    }
}

void WifiDriver::requestCheckDevicePassword(const std::string& address, const std::string& password)
{
    if (address != "" && password != "") {
        mWifiDiscovery->requestCheckPassword(address, password);
    }
}

void WifiDriver::requestUpdatePairedDevices(service::WifiDeviceInfo* newDevice) {
    if (newDevice->address == "") {
        return;
    }
    mWifiPairing->appendNewPairedDevice(newDevice);

}

}