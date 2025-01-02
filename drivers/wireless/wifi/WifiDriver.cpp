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

    std::list<service::WifiDeviceInfo*> pairedList = mWifiPairing->getPairedDeviceList();

    for (std::list<service::WifiDeviceInfo*>::iterator it = pairedList.begin(); it != pairedList.end(); it++) {
        if ((*it) != nullptr) {
            if (true == compareTexture(address, (*it)->address)) {
                mWifiPairing->requestConnectDevice(*it);
                mConnectingDeviceType = ConnectingDeviceType::Paired;
                return;
            }
        }
    }

    std::list<service::WifiDeviceInfo*> discoveryList = mWifiDiscovery->getWifiDiscoveryList();

    for (std::list<service::WifiDeviceInfo*>::iterator it = discoveryList.begin(); it != discoveryList.end(); it++) {
        if ((*it) != nullptr) {
            if (true == compareTexture(address, (*it)->address)) {
                mConnectingDeviceType = ConnectingDeviceType::Discovery;
                mWifiDiscovery->requestConnectDevice(*it);
                return;
            }
        }
    }
}

void WifiDriver::requestForgetDevice(const std::string &address)
{
    if (true == compareTexture(address, mConnectedDevice->address)) {
        mWifiDiscovery->addDiscoveryDevice(mConnectedDevice);
        service::WifiDeviceInfo* emptyDevice = new service::WifiDeviceInfo("", false, "",
                                                                           "", false, static_cast<service::WifiSpeedMode>(0));
        mConnectedDevice = emptyDevice;
        onConnectedDeviceUpdated.emit(mConnectedDevice);

    } else {
        std::list<service::WifiDeviceInfo*> pairedList = mWifiPairing->getPairedDeviceList();

        for (std::list<service::WifiDeviceInfo*>::iterator it = pairedList.begin(); it != pairedList.end(); it++) {
            if ((*it) != nullptr) {
                if (true == compareTexture(address, (*it)->address)) {
                    mWifiDiscovery->addDiscoveryDevice(*it);
                    mWifiPairing->removePairedDevice((*it)->address);
                    return;
                }
            }
        }
    }
}

bool WifiDriver::compareTexture(const std::string &firstDevice, const std::string &secondDevice)
{
    std::string device1 = firstDevice;
    std::string device2 = secondDevice;

    if (device1 == "" || device2 == "")
        return false;

    if (device1.find('\0') != std::string::npos) {
        device1.erase(device1.end() - 1);
    }

    if (device2.find('\0') != std::string::npos) {
        device2.erase(device1.end() - 1);
    }

    if (device1 == device2) {
        return true;
    }
    return false;
}

void WifiDriver::requestCancelConnecting()
{
    if (mConnectingDeviceType == ConnectingDeviceType::Paired) {
        mWifiPairing->cancelConnecting();
    } else if (mConnectingDeviceType == ConnectingDeviceType::Discovery) {
        mWifiDiscovery->cancelConnecting();
    }
    mConnectingDeviceType == ConnectingDeviceType::None;
}

void WifiDriver::requestCheckDevicePassword(const std::string& address, const std::string& password)
{
    if (address != "" && password != "") {
        mWifiDiscovery->requestCheckPassword(address, password);
    }
}

}
