#include "WifiServiceDeploy.h"
#include <utils/Logger.h>

namespace service {

static WifiServiceDeploy* gInstance = nullptr;

WifiServiceDeploy::WifiServiceDeploy()
{
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

void WifiServiceDeploy::responseServiceReady(const std::string& clientName)
{
    std::lock_guard<std::mutex> lock(mMutex);
    mMqSender.startMsq(base::msq::Msq_Wifi_RespReady);
    mMqSender.sendMsq(clientName);
}

void WifiServiceDeploy::responsePairedListUpdated()
{
    mClientManager.deploy([this](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Msq_Wifi_RespPairedListUpdated);
        mMqSender.sendMsq(mqName);
    });
}

void WifiServiceDeploy::responseConnectedDeviceUpdated()
{
    mClientManager.deploy([this](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Msq_Wifi_RespConnectedDeviceUpdated);
        mMqSender.sendMsq(mqName);
    });
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

void WifiServiceDeploy::responseDiscoveryDeviceUpdated(service::WifiDiscoveryDeviceInfo* device)
{
    mClientManager.deploy([this, device](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Msq_Wifi_RespDiscoveryDeviceUpdated);
        mMqSender.addParam(device->name.c_str());
        mMqSender.addParam(device->address.c_str());
        mMqSender.addParam(device->privateAddr);
        mMqSender.addParam(static_cast<int>(device->speedmode));
        mMqSender.sendMsq(mqName);
    });
}

void WifiServiceDeploy::responseAuthencatePassword(const std::string& address) {
    mClientManager.deploy([this, address](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Msq_Wifi_RespAuthencatePassword);
        mMqSender.addParam(address.c_str());
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
