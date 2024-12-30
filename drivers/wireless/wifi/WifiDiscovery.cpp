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
    utils::VariantList dataList = mWifiDriver->mRepo[common::ParameterIndex::Wifi_Discovery];
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

    mWifiDriver->mRepo[common::ParameterIndex::Wifi_Discovery] = discoveryList;
}

void WifiDiscovery::handleConnectDevice(milliseconds delta)
{
    if (mConnectDeviceFlag)
    {
        mTime += delta;
        if (mStep == 0)
        {
            mAuthenStatus = service::WifiAuthenDeviceStatus::CheckingSSID;
            //mEvent.push(WifiEvent::AuthenStatus);
            mTime = milliseconds(0);
            mStep++;
        }
        if (mStep == 1 && mTime > milliseconds(2000))
        {
            /** CASE: Authen Fail 
             *  mAuthenStatus = service::WifiAuthenDeviceStatus::Fail;
                //mEvent.push(WifiEvent::AuthenStatus);
                mTime = milliseconds(0);
                mStep = 0;
                mConnectDeviceFlag = false;
             **/

            mAuthenStatus = service::WifiAuthenDeviceStatus::CheckedSSID;
            //mEvent.push(WifiEvent::AuthenStatus);
            mTime = milliseconds(0);
            mStep++;
        }
        if (mStep == 2 && mTime > milliseconds(500))
        {
            mAuthenStatus = service::WifiAuthenDeviceStatus::Authencating;
            //mEvent.push(WifiEvent::AuthenStatus);
            mTime = milliseconds(0);
            mStep++;
        }
        if (mStep == 3 && mTime > milliseconds(2000))
        {
            mAuthenStatus = service::WifiAuthenDeviceStatus::AuthenSuccess;
            //mEvent.push(WifiEvent::AuthenStatus);
            mTime = milliseconds(0);
            mStep++;
        }
        if (mStep == 4 && mTime > milliseconds(1000))
        {
            // if (peripherals::WifiDriver::instance().addPairedDevice(peripherals::WifiDriver::instance().getConnectedDevice()))
            // {
            //     mEvent.push(WifiEvent::PairedDeviceChange);
            // }
            mTime = milliseconds(0);
            mStep++;
        }
        if (mStep == 5 && mTime > milliseconds(100))
        {
            // auto newConnectedDevice = peripherals::WifiDriver::instance().queryWifiDevice(mDiscoryDeviceAddr);
            // peripherals::WifiDriver::instance().setConnectedDevice(newConnectedDevice);
            //mEvent.push(WifiEvent::UpdateConnectedDevice);
            mTime = milliseconds(0);
            mStep = 0;
            mConnectDeviceFlag = false;
        }
        
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

void WifiDiscovery::requestConnectDevice(const std::string& addr)
{
    if (mConnectDeviceFlag) {
        mStep = 0;
        mTime = milliseconds(0);
    }

    mDiscoryDeviceAddr = addr;
    mConnectDeviceFlag = true;
}

}
