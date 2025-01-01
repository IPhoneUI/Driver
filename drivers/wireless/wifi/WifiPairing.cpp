#include "WifiPairing.h"
#include "WifiDriver.h"

#define SERVICE_NAME base::utils::highlightString("WifiParing").c_str()

namespace driver {

WifiPairing::WifiPairing(WifiDriver* driver)
    : mWifiDriver(driver)
{
}

void WifiPairing::readData()
{    
    utils::VariantList dataList = mWifiDriver->mRepository[common::ParameterIndex::Wifi_Paired];
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
        mPairedDevices.push_back(device);
    }
}

void WifiPairing::writeBuffer()
{
    utils::VariantList pairedList;
    for (const auto& device : mPairedDevices)
    {
        std::unordered_map<std::string, utils::Variant> item;
        service::WifiDeviceInfo* data = device;
        item["password"] = data->password;
        item["autoconnect"] = data->autoconnectstatus;
        item["name"] = data->name;
        item["address"] = data->address;
        item["privateaddress"] = data->privateAddr;
        item["wifisignal"] = static_cast<int>(data->speedmode);

        pairedList.push(item);
    }

    mWifiDriver->mRepository[common::ParameterIndex::Wifi_Paired] = pairedList;
}

void WifiPairing::execute(milliseconds delta)
{
    if (mPairingFlag)
    {
        mTime += delta;
        if (mStep == 0)
        {
            mWifiDriver->onPairedDeviceListUpdated.emit(mPairedDevices);
            mAuthenStatus = service::WifiAuthenDeviceStatus::Authencating;
            mTime = milliseconds(0);
            mStep++;
        }
        if (mStep == 1 && mTime > milliseconds(2000))
        {
            mAuthenStatus = service::WifiAuthenDeviceStatus::AuthenSuccess;
            mTime = milliseconds(0);
            mStep++;
        }
        if (mStep == 2 && mTime > milliseconds(1000))
        {
            mTime = milliseconds(0);
            mStep++;
        }
        if (mStep == 3 && mTime > milliseconds(100))
        {
            mWifiDriver->mConnectedDevice = mPairingDevice;
            mWifiDriver->onConnectedDeviceUpdated.emit(mPairingDevice);
            mWifiDriver->onPairedDeviceListUpdated.emit(mPairedDevices);
            mTime = milliseconds(0);
            mStep = 0;
            mPairingFlag = false;
        }
        
    }
}

void WifiPairing::setPairingDevice(service::WifiDeviceInfo* device) {
    if (device != nullptr) {
        mPairingDevice = device;
    }
}

std::list<service::WifiDeviceInfo*> WifiPairing::getPairedDeviceList()
{
    return mPairedDevices;
}

void WifiPairing::requestConnectDevice(const std::string& addr)
{
    if (mPairingFlag) {
        mStep = 0;
        mTime = milliseconds(0);
    }

    mPairingFlag = true;
    mPairingAddr = addr;
}

void WifiPairing::appendNewPairedDevice(service::WifiDeviceInfo* newDevice) {
    if (newDevice != nullptr) {
        mPairedDevices.emplace_back(newDevice);
    }
}

}