#include "WifiServiceDeploy.h"
#include <utils/Logger.h>

namespace service {

static WifiServiceDeploy* gInstance = nullptr;

WifiServiceDeploy::WifiServiceDeploy()
{
    mProvider = base::shm::WifiProvider::instance();
}

WifiServiceDeploy::~WifiServiceDeploy()
{
}

WifiServiceDeploy* WifiServiceDeploy::instance()
{
    if (gInstance == nullptr)
    {
        gInstance = new WifiServiceDeploy();
    }
    return gInstance;
}

void WifiServiceDeploy::responseDriverReady(const std::string& clientName)
{
    std::lock_guard<std::mutex> lock(mMutex);
    mMqSender.startMsq(base::msq::Msq_Wifi_RespReady);
    mMqSender.sendMsq(clientName);
}

void WifiServiceDeploy::responseSync(const std::string& clientName)
{
    {
        bool status = mProvider->getWifiStatus();
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Msq_Wifi_RespStatusUpdated);
        mMqSender.addParam(status);
        mMqSender.sendMsq(clientName);
    }
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Msq_Wifi_RespPairedListUpdated);
        mMqSender.sendMsq(clientName);
    }
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Msq_Wifi_RespConnectedDeviceUpdated);
        mMqSender.sendMsq(clientName);
    }
}

void WifiServiceDeploy::responseChangeWifiStatus(bool status)
{
    mClientManager.deploy([this, status](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Msq_Wifi_RespStatusUpdated);
        mMqSender.addParam(status);
        mMqSender.sendMsq(mqName);
    });
}

void WifiServiceDeploy::responseDiscoveryDeviceUpdated(const base::shm::WifiDiscoveryDeviceShmem& device)
{
    mClientManager.deploy([this, device](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Msq_Wifi_RespDiscoveryDeviceUpdated);
        mMqSender.addParam(device.name);
        mMqSender.addParam(device.address);
        mMqSender.addParam(device.privateAddr);
        mMqSender.addParam(device.speedmode);
        mMqSender.sendMsq(mqName);
    });
}

void WifiServiceDeploy::responseCheckDevicePassword(bool result)
{
    mClientManager.deploy([this, result](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Msq_Wifi_RespCheckPasswordUpdated);
        mMqSender.addParam(result);
        mMqSender.sendMsq(mqName);
    });
}

}
