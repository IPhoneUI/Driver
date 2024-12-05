#include "WifiDeploy.h"
#include <utils/Logger.h>

namespace driver {

static WifiDeploy* gInstance = nullptr;

WifiDeploy::WifiDeploy()
    : common::BaseDeploy(service::Msq_Wifi_Type)
{
    mProvider = WifiProvider::instance();
}

WifiDeploy::~WifiDeploy()
{
}

WifiDeploy* WifiDeploy::instance()
{
    if (gInstance == nullptr)
    {
        gInstance = new WifiDeploy();
    }
    return gInstance;
}

void WifiDeploy::responseDriverReady(const std::string& clientName)
{
    std::lock_guard<std::mutex> lock(mMutex);
    mMqSender.startMsq(service::Wifi_Ready);
    mMqSender.addParam(mDriverType);
    mMqSender.sendMsq(clientName);
}

void WifiDeploy::responseSync(const std::string& clientName)
{
    {
        bool status = mProvider->getWifiStatus();
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(service::Wifi_StatusUpdated);
        mMqSender.addParam(mDriverType);
        mMqSender.addParam(status);
        mMqSender.sendMsq(clientName);
    }
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(service::Wifi_PairedListUpdated);
        mMqSender.addParam(mDriverType);
        mMqSender.sendMsq(clientName);
    }
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(service::Wifi_ConnectedDeviceUpdated);
        mMqSender.addParam(mDriverType);
        mMqSender.sendMsq(clientName);
    }
}

void WifiDeploy::responseChangeWifiStatus(bool status)
{
    mClientManager.execute(service::Msq_Wifi_Client, [this, status](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(service::Wifi_StatusUpdated);
        mMqSender.addParam(mDriverType);
        mMqSender.addParam(status);
        mMqSender.sendMsq(mqName);
    });
}

}