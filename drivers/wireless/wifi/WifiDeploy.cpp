#include "WifiDeploy.h"
#include <utils/Logger.h>

namespace driver {

static WifiDeploy* gInstance = nullptr;

WifiDeploy::WifiDeploy()
{
    mProvider = base::shm::WifiProvider::instance();
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
    mMqSender.startMsq(base::msq::Wifi_Ready);
    mMqSender.sendMsq(clientName);
}

void WifiDeploy::responseSync(const std::string& clientName)
{
    {
        bool status = mProvider->getWifiStatus();
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Wifi_RespStatusUpdated);
        mMqSender.addParam(status);
        mMqSender.sendMsq(clientName);
    }
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Wifi_RespPairedListUpdated);
        mMqSender.sendMsq(clientName);
    }
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Wifi_RespConnectedDeviceUpdated);
        mMqSender.sendMsq(clientName);
    }
}

void WifiDeploy::responseChangeWifiStatus(bool status)
{
    // mClientManager.deploy(base::msq::Msq_Wifi_Client, [this, status](std::string mqName) {
    //     std::lock_guard<std::mutex> lock(mMutex);
    //     mMqSender.startMsq(base::msq::Wifi_RespStatusUpdated);
    //     mMqSender.addParam(mDriverType);
    //     mMqSender.addParam(status);
    //     mMqSender.sendMsq(mqName);
    // });
}

void WifiDeploy::responseDiscoveryDeviceUpdated(const base::shm::WifiDiscoveryDeviceShmem& device)
{
    // mClientManager.deploy(base::msq::Msq_Wifi_Client, [this, device](std::string mqName) {
    //     std::lock_guard<std::mutex> lock(mMutex);
    //     mMqSender.startMsq(base::msq::Wifi_RespDiscoveryDeviceUpdated);
    //     mMqSender.addParam(mDriverType);
    //     mMqSender.addParam(device.name);
    //     mMqSender.addParam(device.address);
    //     mMqSender.addParam(device.privateAddr);
    //     mMqSender.addParam(device.speedmode);
    //     mMqSender.sendMsq(mqName);
    // });
}

void WifiDeploy::responseCheckDevicePassword(bool result)
{
    // mClientManager.deploy(base::msq::Msq_Wifi_Client, [this, result](std::string mqName) {
    //     std::lock_guard<std::mutex> lock(mMutex);
    //     mMqSender.startMsq(base::msq::Wifi_RespCheckPassword);
    //     mMqSender.addParam(mDriverType);
    //     mMqSender.addParam(result);
    //     mMqSender.sendMsq(mqName);
    // });
}

}