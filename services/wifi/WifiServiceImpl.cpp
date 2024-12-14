#include "WifiServiceImpl.h"
#include <utils/Logger.h>
#include <wifi/WifiPairing.h>
#include <wifi/WifiDiscovery.h>

namespace service {

WifiServiceImpl::WifiServiceImpl()
{
    mDeploy = WifiServiceDeploy::instance();
    mProvider = base::shm::WifiProvider::instance();
}

void WifiServiceImpl::onMsqReceived()
{
    std::vector<std::string> messages = mMqReceiver.receive("/wifiReq");
    if (!messages.empty()) 
    {
        base::msq::Msq_Wifi_Req type = static_cast<base::msq::Msq_Wifi_Req>(mMqReceiver.get<int>(messages[0]));
        switch (type) {
        case base::msq::Msq_Wifi_ReqRegisterClient: {
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            registerClient(clientName);
            break;
        }
        case base::msq::Msq_Wifi_ReqSync: {
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            mDeploy->responseSync(clientName);
            break;
        }
        case base::msq::Msq_Wifi_ReqChangeWifiStatus: {
            bool status = mMqReceiver.get<bool>(messages[1]);
            requestChangeWifiStatus(status);
            break;
        }
        case base::msq::Msq_Wifi_ReqStartDiscovery: {
            // driver::WifiDiscovery::instance().startDiscovery();
            break;
        }
        case base::msq::Msq_Wifi_ReqCheckPassword: {
            std::string address = mMqReceiver.get<std::string>(messages[1]);
            std::string password = mMqReceiver.get<std::string>(messages[2]);
            requestCheckDevicePassword(address, password);
            break;
        }
        case base::msq::Msq_Wifi_ReqConnectDevice: {
            std::string address = mMqReceiver.get<std::string>(messages[1]);
            requestConnectDevice(address);
        }
        }
    }
}

// void WifiServiceImpl::execute(milliseconds delta)
// {
//     // driver::WifiDiscovery::instance().execute(delta);
//     // driver::WifiPairing::instance().execute(delta);
// }

void WifiServiceImpl::initialize()
{
    LOG_INFO("WifiServiceImpl initialize");
    mProvider->initialize();
}

void WifiServiceImpl::finialize()
{
    LOG_INFO("WifiServiceImpl finialize");
    mProvider->closeShmem();
}

void WifiServiceImpl::registerClient(const std::string& clientName)
{
    if (mDeploy->registerClient(clientName))
    {
        mDeploy->responseServiceReady(clientName);
    }
}

void WifiServiceImpl::requestChangeWifiStatus(bool status)
{
    auto result = mProvider->setWifiStatus(status);
    if (result == base::shm::DataSetResult_Valid)
    {
        mDeploy->responseChangeWifiStatus(status);
    }
}

void WifiServiceImpl::requestConnectDevice(const std::string& address)
{
    LOG_INFO("Request connect device with address: %s", address.c_str());
    for (const auto& item : base::shm::WifiProvider::instance()->getPairedDeviceList())
    {
        if (item.deviceinfo.address == address)
        {
            // driver::WifiPairing::instance().requestConnectDevice(address);
            return;
        }
    }
    for (const auto& item : base::shm::WifiProvider::instance()->getPairedDeviceList())
    {
        if (item.deviceinfo.address == address)
        {
            // driver::WifiDiscovery::instance().requestConnectDevice(address);
            return;
        }
    }
}

void WifiServiceImpl::requestCheckDevicePassword(const std::string& address, const std::string& password)
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