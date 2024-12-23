#include "WifiDiscovery.h"
#include "WifiDriver.h"

#define SERVICE_NAME base::utils::highlightString("WifiParing").c_str()

namespace driver {

WifiDiscovery::WifiDiscovery(WifiDriver* driver)
    : mWifiDriver(driver)
    , mDevice(new service::WifiDiscoveryDeviceInfo())
{
}

void WifiDiscovery::execute(milliseconds delta)
{
    handleDiscovering(delta);
    handleConnectDevice(delta);
}

void WifiDiscovery::readData()
{
    common::DataRepoManager& dataRepo = common::DataRepoManager::instance();

    if (dataRepo.isReady())
    {
        common::Repository& repo = dataRepo.getRepository("wifi");
        auto dataMap = repo[common::ParameterIndex::Wifi_Data].toList();
        
        int count = 0;
        for (int i = 1; i < 6; ++i) 
        {   
            auto item = *std::next(dataMap.begin(), i);
            std::string name = std::string(item["name"]);
            std::string address = std::string(item["address"]);
            service::WifiDiscoveryDeviceInfo* deviceInfo = new service::WifiDiscoveryDeviceInfo(
                name, 
                address, 
                item["privateaddress"], 
                static_cast<service::WifiSpeedMode>(int(item["wifisignal"]))
            );
            mDiscoryDeviceList.push_back(deviceInfo);
            ++count;
        }
    }
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
            mTime = milliseconds(0);
            mStep++;
        }
        if (mStep == 2 && mTime > milliseconds(500))
        {
            // Check password

            mAuthenStatus = service::WifiAuthenDeviceStatus::Authencating;
            mTime = milliseconds(0);
            mStep++;
        }
        if (mStep == 3 && mTime > milliseconds(2000))
        {
            mAuthenStatus = service::WifiAuthenDeviceStatus::AuthenSuccess;
            mTime = milliseconds(0);
            mStep++;
        }
        if (mStep == 4 && mTime > milliseconds(1000))
        {
            mTime = milliseconds(0);
            mStep++;
        }
        if (mStep == 5 && mTime > milliseconds(100))
        {
            LOG_INFO("THAIVD -- Finished");
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
        if (mStep == mDiscoryDeviceList.size())
        {
            mDiscoveringTime = milliseconds(0);
            mDiscoveryFlag = false;
            mStep = 0;
        }
        else if (mDiscoveringTime > milliseconds(1000))
        {
            auto it = mDiscoryDeviceList.begin();
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

void WifiDiscovery::requestConnectDevice(service::WifiDiscoveryDeviceInfo* device, const std::string& password)
{
    if (mConnectDeviceFlag) {
        mStep = 0;
        mTime = milliseconds(0);
    }
    LOG_INFO("THAIVD --- start Connect Device add: %s - password: %s", device->address.c_str(), password.c_str());
    mDiscoryDeviceAddr = device->address;
    mPasswordDevice = password;
    mConnectDeviceFlag = true;
}

void WifiDiscovery::setWaitPairDevice(service::WifiDiscoveryDeviceInfo *device)
{
    if (device == nullptr)
        return;
    mDeviceWaitPair = device;
}

service::WifiDiscoveryDeviceInfo *WifiDiscovery::getDiscoveryDeviceInfo(const std::string &address)
{
    for (auto it : mDiscoryDeviceList) {
        if (it->address == address) {
            return it;
        }
    }
    return nullptr;
}

}
