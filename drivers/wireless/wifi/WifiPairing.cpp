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
        service::WifiDeviceInfo* device = new service::WifiDeviceInfo(
            item["password"], 
            item["autoconnect"], 
            item["name"], 
            item["address"], 
            item["privateaddress"], 
            static_cast<service::WifiSpeedMode>(int(item["wifisignal"]))
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
            // mEvent.push(WifiEvent::AuthenStatus);
            mTime = milliseconds(0);
            mStep++;
        }
        if (mStep == 1 && mTime > milliseconds(2000))
        {
            /** CASE: Authen Fail 
             *  mAuthenStatus = WifiAuthenDeviceStatus::Fail;
                mEvent.push(WifiEvent::AuthenStatus);
                mTime = milliseconds(0);
                mStep = 0;
                mPairingFlag = false;
             **/ 
            

            mAuthenStatus = service::WifiAuthenDeviceStatus::AuthenSuccess;
            // auto pairList = base::shm::WifiProvider::instance()->getPairedDeviceList();
            // for (const auto& device : pairList)
            // {
            //     if (std::strcmp(device.password, mPairingAddr.c_str()))
            //     {
            //         // Todo
            //     }
            // }
            // mEvent.push(WifiEvent::AuthenStatus);
            mTime = milliseconds(0);
            mStep++;
        }
        if (mStep == 2 && mTime > milliseconds(1000))
        {
            // if (peripherals::WifiDriver::instance().removePairedDevice(mPairingAddr))
            // {
            //     mEvent.push(WifiEvent::PairedDeviceChange);
            // }
            mTime = milliseconds(0);
            mStep++;
        }
        if (mStep == 3 && mTime > milliseconds(100))
        {
            // if (peripherals::WifiDriver::instance().addPairedDevice(peripherals::WifiDriver::instance().getConnectedDevice()))
            // {
            //     mEvent.push(WifiEvent::PairedDeviceChange);
            // }
            // peripherals::WifiDriver::instance().setConnectedDevice(mPairingDevice);
            // mEvent.push(WifiEvent::UpdateConnectedDevice);
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

void WifiPairing::requestConnectDevice(const std::string& addr)
{
    if (mPairingFlag) {
        mStep = 0;
        mTime = milliseconds(0);
    }

    mPairingFlag = true;
    mPairingAddr = addr;
}

}