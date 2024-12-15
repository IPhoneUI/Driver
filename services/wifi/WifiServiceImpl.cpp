#include "WifiServiceImpl.h"
#include <utils/Logger.h>
#include <wifi/WifiPairing.h>
#include <wifi/WifiDiscovery.h>

namespace service {

WifiServiceImpl::WifiServiceImpl()
{
    mDeploy = WifiServiceDeploy::instance();
    mProvider = base::shm::WifiProvider::instance();
    mWifiDriver = driver::WifiDriver::getInstance();
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
            requestSync();
            break;
        }
        case base::msq::Msq_Wifi_ReqChangeWifiStatus: {
            bool status = mMqReceiver.get<bool>(messages[1]);
            mWifiDriver->requestChangeWifiStatus(status);
            break;
        }
        case base::msq::Msq_Wifi_ReqStartDiscovery: {
            mWifiDriver->startDiscovery();
            break;
        }
        case base::msq::Msq_Wifi_ReqCheckPassword: {
            std::string address = mMqReceiver.get<std::string>(messages[1]);
            std::string password = mMqReceiver.get<std::string>(messages[2]);
            mWifiDriver->requestCheckDevicePassword(address, password);
            break;
        }
        case base::msq::Msq_Wifi_ReqConnectDevice: {
            std::string address = mMqReceiver.get<std::string>(messages[1]);
            mWifiDriver->requestConnectDevice(address);
        }
        }
    }
}

void WifiServiceImpl::initialize()
{
    LOG_INFO("WifiServiceImpl initialize");
    Connection::connect(mWifiDriver->onDriverReady, std::bind(&WifiServiceImpl::onWifiDriverReady, this));
    Connection::connect(mWifiDriver->onWifiStatusUpdated, std::bind(&WifiServiceImpl::onWifiStatusUpdated, this, std::placeholders::_1));
    Connection::connect(mWifiDriver->onPairedDeviceListUpdated, std::bind(&WifiServiceImpl::onPairedDeviceListUpdated, this, std::placeholders::_1));
    Connection::connect(mWifiDriver->onConnectedDeviceUpdated, std::bind(&WifiServiceImpl::onConnectedDeviceUpdated, this, std::placeholders::_1));
    Connection::connect(mWifiDriver->onAddDiscoryDeviceInfo, std::bind(&WifiServiceImpl::onAddDiscoryDeviceInfo, this, std::placeholders::_1));

    mWifiDriver->connectDriver();
}

void WifiServiceImpl::finialize()
{
    LOG_INFO("WifiServiceImpl finialize");
}

void WifiServiceImpl::registerClient(const std::string& clientName)
{
    if (mDeploy->registerClient(clientName))
    {
        mDeploy->responseServiceReady(clientName);
    }
}

void WifiServiceImpl::requestSync()
{
    bool wifiStatus = mWifiDriver->getWifiStatus();
    mDeploy->responseChangeWifiStatus(wifiStatus);

    auto pairedList = mWifiDriver->getPairedDeviceList();
    onPairedDeviceListUpdated(pairedList);

    auto connectedDevice = mWifiDriver->getConnectedDevice();
    onConnectedDeviceUpdated(connectedDevice);
}

void WifiServiceImpl::onWifiDriverReady()
{
    // To do
}

void WifiServiceImpl::onWifiStatusUpdated(bool status)
{
    mDeploy->responseChangeWifiStatus(status);
}

void WifiServiceImpl::onPairedDeviceListUpdated(const std::list<service::WifiDeviceInfo*>& list)
{
    bool result = mProvider->setPairedDeviceList(list);
    if (result)
    {
        mDeploy->responsePairedListUpdated();
    }
}

void WifiServiceImpl::onConnectedDeviceUpdated(service::WifiDeviceInfo* device)
{
    bool result = mProvider->setConnectedDevice(device);
    if (result)
    {
        mDeploy->responseConnectedDeviceUpdated();
    }
}

void WifiServiceImpl::onAddDiscoryDeviceInfo(service::WifiDiscoveryDeviceInfo* device)
{
    mDeploy->responseDiscoveryDeviceUpdated(device);
}

}