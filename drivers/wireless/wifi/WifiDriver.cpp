#include "WifiDriver.h"
#include <utils/Logger.h>
#include "WifiDiscovery.h"
#include "WifiPairing.h"

namespace driver {

WifiDriver::WifiDriver()
{
    mProvider = base::shm::WifiProvider::instance();
    mDeploy = WifiDeploy::instance();
}

void WifiDriver::onMsqReceived()
{
    std::vector<std::string> messages = mMqReceiver.receive("/wifiReq");
    if (!messages.empty()) 
    {
        base::msq::Msq_WifiReq type = static_cast<base::msq::Msq_WifiReq>(mMqReceiver.get<int>(messages[0]));
        switch (type) {
        case base::msq::Wifi_RegisterClient: {
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            registerClient(base::msq::Msq_Wifi_Client, clientName);
            break;
        }
        case base::msq::Wifi_ReqSync: {
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            mDeploy->responseSync(clientName);
            break;
        }
        case base::msq::Wifi_ReqChangeWifiStatus: {
            bool status = mMqReceiver.get<bool>(messages[1]);
            requestChangeWifiStatus(status);
            break;
        }
        case base::msq::Wifi_ReqStartDiscovery: {
            WifiDiscovery::instance().startDiscovery();
            break;
        }
        case base::msq::Wifi_ReqCheckPassword: {
            std::string address = mMqReceiver.get<std::string>(messages[1]);
            std::string password = mMqReceiver.get<std::string>(messages[2]);
            requestCheckDevicePassword(address, password);
            break;
        }
        case base::msq::Wifi_ReqConnectDevice: {
            std::string address = mMqReceiver.get<std::string>(messages[1]);
            requestConnectDevice(address);
        }
        }
    }
}

void WifiDriver::execute(milliseconds delta)
{
    WifiDiscovery::instance().execute(delta);
    WifiPairing::instance().execute(delta);
}

void WifiDriver::initialize()
{
    LOG_INFO("WifiDriver initialize");
    mProvider->initialize();
}

void WifiDriver::finialize()
{
    LOG_INFO("WifiDriver finialize");
}

void WifiDriver::registerClient(base::msq::Msq_Client clientId, const std::string& clientName)
{
    // if (mDeploy->registerClient(clientId, clientName))
    // {
    //     mDeploy->responseDriverReady(clientName);
    // }
}

void WifiDriver::requestChangeWifiStatus(bool status)
{
    auto result = mProvider->setWifiStatus(status);
    if (result == base::shm::DataSetResult_Valid)
    {
        mDeploy->responseChangeWifiStatus(status);
    }
}

void WifiDriver::requestConnectDevice(const std::string& address)
{
    LOG_INFO("Request connect device with address: %s", address.c_str());
    for (const auto& item : base::shm::WifiProvider::instance()->getPairedDeviceList())
    {
        if (item.deviceinfo.address == address)
        {
            WifiPairing::instance().requestConnectDevice(address);
            return;
        }
    }
    for (const auto& item : base::shm::WifiProvider::instance()->getPairedDeviceList())
    {
        if (item.deviceinfo.address == address)
        {
            WifiDiscovery::instance().requestConnectDevice(address);
            return;
        }
    }
}

void WifiDriver::requestCheckDevicePassword(const std::string& address, const std::string& password)
{
    std::function<bool(std::string, std::string, std::list<base::shm::WifiDeviceShmem>)> checkPassword = [](std::string address, std::string password, std::list<base::shm::WifiDeviceShmem> discoveryList) -> bool 
    {
        for (auto it = discoveryList.begin(); it != discoveryList.end(); it++) 
        {
            if (std::strcmp((*it).deviceinfo.address, address.c_str()) && std::strcmp((*it).password, password.c_str())) 
            {
                return true;
            }
        }
        return false;
    };

    bool result = false;
    std::list<base::shm::WifiDeviceShmem> list = base::shm::WifiProvider::instance()->getPairedDeviceList();
    if (checkPassword(address, password, list)) 
    {
        result = true;
    }

    mDeploy->responseCheckDevicePassword(result);
}

}