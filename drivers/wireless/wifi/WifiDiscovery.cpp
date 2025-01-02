#include "WifiDiscovery.h"
#include "WifiDriver.h"

#define SERVICE_NAME base::utils::highlightString("WifiParing").c_str()

namespace driver {

WifiDiscovery::WifiDiscovery(WifiDriver* driver)
    : mWifiDriver(driver)
    , mDevice(new service::WifiDeviceInfo())
{
}

void WifiDiscovery::execute(milliseconds delta)
{
    handleDiscovering(delta);
    handleConnectDevice(delta);
}

void WifiDiscovery::readData()
{
    utils::VariantList dataList = mWifiDriver->mRepository[common::ParameterIndex::Wifi_Discovery];
    for (const auto &deviceItem : dataList) 
    {   
        std::unordered_map<std::string, utils::Variant> item = deviceItem;
        service::WifiDeviceInfo* device = new service::WifiDeviceInfo(
            item["password"], 
            item["autoconnect"], 
            item["name"], 
            item["address"], 
            item["privateaddress"], 
            static_cast<service::WifiSpeedMode>(int(item["wifisignal"]))
        );
        mDiscoveryDevices.push_back(device);
    }
}

void WifiDiscovery::writeBuffer()
{
    utils::VariantList discoveryList;
    for (const auto& device : mDiscoveryDevices)
    {
        std::unordered_map<std::string, utils::Variant> item;
        service::WifiDeviceInfo* data = device;
        item["password"] = data->password;
        item["autoconnect"] = data->autoconnectstatus;
        item["name"] = data->name;
        item["address"] = data->address;
        item["privateaddress"] = data->privateAddr;
        item["wifisignal"] = static_cast<int>(data->speedmode);

        discoveryList.push(item);
    }

    mWifiDriver->mRepository[common::ParameterIndex::Wifi_Discovery] = discoveryList;
}

void WifiDiscovery::handleConnectDevice(milliseconds delta)
{
    if (mConnectDeviceFlag)
    {
        mTime += delta;
        if (mStep == 0)
        {
            mAuthenStatus = service::WifiAuthenDeviceStatus::CheckingSSID;
            mTime = milliseconds(0);
            mStep++;
        }
        if (mStep == 1 && mTime > milliseconds(200))
        {
            mAuthenStatus = service::WifiAuthenDeviceStatus::CheckedSSID;
            mTime = milliseconds(0);
            mStep++;
        }
        if (mStep == 2 && mTime > milliseconds(300))
        {
            mAuthenStatus = service::WifiAuthenDeviceStatus::Authencating;
            mTime = milliseconds(0);
            mStep++;
        }
        if (mStep == 3 && mTime > milliseconds(500)) 
        {
            for (std::list<service::WifiDeviceInfo*>::iterator it = mDiscoveryDevices.begin(); it != mDiscoveryDevices.end(); it++) {
                if (mDiscoryDeviceAddr == (*it)->address) {
                    mPairingDevice = (*it);
                    mAuthenStatus = service::WifiAuthenDeviceStatus::VerifyPassword;
                    mConnectDeviceFlag = false;
                    break;
                } else {
                    mAuthenStatus = service::WifiAuthenDeviceStatus::Fail;
                }
            }
            mTime = milliseconds(0);
            mStep++;
        }
        if (mStep == 4 && mTime > milliseconds(500))
        {
            if (mAuthenStatus == service::WifiAuthenDeviceStatus::VerifyPassword) {
                int result = mPassword.compare(0, mPassword.size() - 1, mPairingDevice->password);
                if (result == 0) {
                    mAuthenStatus = service::WifiAuthenDeviceStatus::AuthenSuccess;
                }

                if (mAuthenStatus != service::WifiAuthenDeviceStatus::AuthenSuccess) {
                    mConnectDeviceFlag = false;
                } else {
                    mStep++;
                }
                mTime = milliseconds(0);
            }
        }
        if (mStep == 5 && mTime > milliseconds(2000)) {
            if (mAuthenStatus == service::WifiAuthenDeviceStatus::AuthenSuccess) {
                removeDiscoveryDevice(mPairingDevice->address);
                if (!mWifiDriver->mConnectedDevice->address.empty()) {
                    mWifiDriver->mConnectedDevice = mPairingDevice;
                }
                mWifiDriver->onConnectedDeviceUpdated.emit(mPairingDevice);

                service::WifiDeviceInfo* newPaired = new service::WifiDeviceInfo(mPairingDevice->password, mPairingDevice->autoconnectstatus, mPairingDevice->name, mPairingDevice->address, mPairingDevice->privateAddr, mPairingDevice->speedmode);
                mWifiDriver->mWifiPairing->appendNewPairedDevice(newPaired);
                mWifiDriver->onCheckPasswordStateUpdated.emit(true);
            } else {
                mWifiDriver->onCheckPasswordStateUpdated.emit(false);
            }
            mTime = milliseconds(0);
            mStep = 0;
            mConnectDeviceFlag = false;
        }
        mWifiDriver->onWifiAuthenDeviceStatusUpdated.emit(mAuthenStatus);
    }
}

void WifiDiscovery::handleDiscovering(milliseconds delta)
{
    static milliseconds mDiscoveringTime = milliseconds(0);
    static int mStep = 0;
    if (mDiscoveryFlag)
    {
        mDiscoveringTime += delta;
        if (mStep == mDiscoveryDevices.size())
        {
            mDiscoveringTime = milliseconds(0);
            mDiscoveryFlag = false;
            mStep = 0;
        }
        else if (mDiscoveringTime > milliseconds(1000))
        {
            auto it = mDiscoveryDevices.begin();
            std::advance(it, mStep);
            mDevice = *it;
            mDevice->name;
            mWifiDriver->onAddDiscoryDeviceInfo.emit(mDevice);
            mDiscoveringTime = milliseconds(0);
            mStep++;
        }
    }
}

void WifiDiscovery::startDiscovery()
{
    mDiscoveryFlag = true;
}

void WifiDiscovery::stopDiscovery()
{
    mDiscoveryFlag = false;
}

void WifiDiscovery::requestConnectDevice(service::WifiDeviceInfo* device)
{
    if (device == nullptr) {
        return;
    }

    if (mConnectDeviceFlag) {
        mStep = 0;
        mTime = milliseconds(0);
    }
    
    mPairingDevice = device;
    mDiscoryDeviceAddr = device->address;
    mConnectDeviceFlag = true;
}

void WifiDiscovery::requestCheckPassword(const std::string& address, const std::string& password) {
    if (mDiscoryDeviceAddr == address) {
        mPassword = password;
        mConnectDeviceFlag = true;
    }
}

void WifiDiscovery::removeDiscoveryDevice(const std::string& addr)
{
    for (std::list<service::WifiDeviceInfo*>::iterator it = mDiscoveryDevices.begin(); it != mDiscoveryDevices.end(); it++) {
        if ((*it)->address == addr) {
            mWifiDriver->onRemoveDiscoryDeviceInfo.emit((*it));
            mDiscoveryDevices.erase(it);
            break;
        }
    }
}

}
