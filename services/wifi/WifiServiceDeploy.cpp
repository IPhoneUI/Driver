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

void WifiServiceDeploy::responseRemoveDiscoryDeviceInfo(service::WifiDeviceInfo* device)
{
    mClientManager.deploy([this, device](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Msq_Wifi_RespRemoveDiscoryDeviceInfoUpdated);
        mMqSender.addParam(device->address.c_str());
        mMqSender.sendMsq(mqName);
    });
}

void WifiServiceDeploy::responseAuthenDeviceStatus(const std::string& addr, const WifiAuthenDeviceStatus &state)
{
    mClientManager.deploy([this, addr, state](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Msq_Wifi_RespAuthenDeviceStatusUpdated);
        mMqSender.addParam(addr.c_str());
        mMqSender.addParam(static_cast<int>(state));
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

void WifiServiceDeploy::responseDiscoveryDeviceUpdated(service::WifiDeviceInfo* device)
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
