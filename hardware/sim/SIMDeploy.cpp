#include "SIMDeploy.h"
#include <utils/Logger.h>

namespace driver {

static SIMDeploy* gInstance = nullptr;

SIMDeploy::SIMDeploy()
    : common::BaseDeploy(service::Msq_SIM_Type)
{
    mProvider = SIMProvider::instance();
}

SIMDeploy::~SIMDeploy()
{
}

SIMDeploy* SIMDeploy::instance()
{
    if (gInstance == nullptr)
    {
        gInstance = new SIMDeploy();
    }
    return gInstance;
}

void SIMDeploy::responseDriverReady(const std::string& clientName)
{
    std::lock_guard<std::mutex> lock(mMutex);
    mMqSender.startMsq(service::SIM_Ready);
    mMqSender.addParam(mDriverType);
    mMqSender.sendMsq(clientName);
}

void SIMDeploy::responseSyncCelNetwork(const std::string& clientName)
{
    {
        std::lock_guard<std::mutex> lock(mMutex);
        bool allowAccess = mProvider->getIsAllowAccess();
        mMqSender.startMsq(service::SIM_CelNetwork_RespChangeAllowAcess);
        mMqSender.addParam(mDriverType);
        mMqSender.addParam(allowAccess);
        mMqSender.sendMsq(clientName);
    }
    {
        std::lock_guard<std::mutex> lock(mMutex);
        bool cellular = mProvider->getIsCellular();
        mMqSender.startMsq(service::SIM_CelNetwork_RespChangeCellulatr);
        mMqSender.addParam(mDriverType);
        mMqSender.addParam(cellular);
        mMqSender.sendMsq(clientName);
    }
    {
        std::lock_guard<std::mutex> lock(mMutex);
        bool maxCompa = mProvider->getIsMaxCompatibility();
        mMqSender.startMsq(service::SIM_CelNetwork_RespChangeMaxCompatibility);
        mMqSender.addParam(mDriverType);
        mMqSender.addParam(maxCompa);
        mMqSender.sendMsq(clientName);
    }
    {
        std::lock_guard<std::mutex> lock(mMutex);
        std::string wifiPassword = mProvider->getWifiPassword();
        mMqSender.startMsq(service::SIM_CelNetwork_RespChangeWifiPassword);
        mMqSender.addParam(mDriverType);
        mMqSender.addParam(wifiPassword.c_str());
        mMqSender.sendMsq(clientName);
    }
}


void SIMDeploy::responseChangeCellularStatus(bool status)
{
    mClientManager.execute(service::Msq_CelNetwork_Client, [this, status](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(service::SIM_CelNetwork_RespChangeCellulatr);
        mMqSender.addParam(mDriverType);
        mMqSender.addParam(status);
        mMqSender.sendMsq(mqName);
    });

}
void SIMDeploy::responseChangeAllowAccess(bool status)
{
    mClientManager.execute(service::Msq_CelNetwork_Client, [this, status](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(service::SIM_CelNetwork_RespChangeAllowAcess);
        mMqSender.addParam(mDriverType);
        mMqSender.addParam(status);
        mMqSender.sendMsq(mqName);
    });
}
void SIMDeploy::responseChangeMaxCompatibility(bool status)
{
    mClientManager.execute(service::Msq_CelNetwork_Client, [this, status](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(service::SIM_CelNetwork_RespChangeMaxCompatibility);
        mMqSender.addParam(mDriverType);
        mMqSender.addParam(status);
        mMqSender.sendMsq(mqName);
    });
}

void SIMDeploy::responseSyncPhoneBook(const std::string& clientName)
{
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(service::SIM_PhoneBook_RespQueryContact);
        mMqSender.addParam(mDriverType);
        mMqSender.sendMsq(clientName);
    }
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(service::SIM_PhoneBook_RespQueryHistory);
        mMqSender.addParam(mDriverType);
        mMqSender.sendMsq(clientName);
    }
}

}