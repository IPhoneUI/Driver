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
    utils::VariantList dataList = mWifiDriver->mRepo[common::ParameterIndex::Wifi_Data];
    int count = 0;
    for (int i = 6; i < 14; ++i) 
    {   
        auto item = *std::next(dataList.begin(), i);
        service::WifiDiscoveryDeviceInfo deviceInfo(
            item["name"], 
            item["address"], 
            item["privateaddress"], 
            static_cast<service::WifiSpeedMode>(int(item["wifisignal"]))
        );
        service::WifiDeviceInfo* device = new service::WifiDeviceInfo(
            item["password"], 
            item["autoconnect"], 
            deviceInfo
        );
        mPairedDeviceList.push_back(device);
        ++count;
    }
}

void WifiPairing::execute(milliseconds delta)
{
    if (mPairingFlag)
    {
        mTime += delta;

        if (mStep == 0)
        {
            mAuthenStatus = service::WifiAuthenDeviceStatus::Authencating;
            mTime = milliseconds(0);
            mStep++;
        }
        if (mStep == 1 && mTime > milliseconds(2000))
        {
            if (mAuthenStatus == service::WifiAuthenDeviceStatus::Authencating && mPairingDevice != nullptr) {
                for (std::list<service::WifiDeviceInfo*>::iterator it = mPairedDeviceList.begin(); it != mPairedDeviceList.end(); it++) {
                    if ((*it) != nullptr) {
                        if ((*it)->deviceinfo.address == mPairingDevice->deviceinfo.address) {
                            if ((*it)->password == mPairingDevice->password) {
                                mAuthenStatus = service::WifiAuthenDeviceStatus::AuthenSuccess;
                                mTime = milliseconds(0);
                                mStep++;
                            } else {
                                mAuthenStatus = service::WifiAuthenDeviceStatus::Fail;
                                mTime = milliseconds(0);
                                mStep = 0;
                                mPairingFlag = false;
                            }
                        }
                    }
                }           
            }
        }
        if (mStep == 2 && mTime > milliseconds(1000))
        {
            mWifiDriver->setConnectedDevice(mPairingDevice);
            mTime = milliseconds(0);
            mStep++;
        }
        if (mStep == 3 && mTime > milliseconds(100))
        {
            mWifiDriver->onConnectedDeviceUpdated.emit(mWifiDriver->getConnectedDevice());
            mTime = milliseconds(0);
            mStep = 0;
            mPairingFlag = false;
        }
        
    }
}

std::list<service::WifiDeviceInfo*> WifiPairing::getPairedDeviceList()
{
    return mPairedDeviceList;
}

void WifiPairing::requestConnectDevice(service::WifiDeviceInfo* device)
{
    if (mPairingFlag) {
        mStep = 0;
        mTime = milliseconds(0);
    }

    mPairingFlag = true;
    mPairingAddr = device->deviceinfo.address;
    mPairingDevice = device;
}

service::WifiDeviceInfo *WifiPairing::getPairedDeviceInfo(const std::string& address) const
{
    for (auto it : mPairedDeviceList) {
        if (it->deviceinfo.address == address) {
            return it;
        }
    }
    return nullptr;
}

}
