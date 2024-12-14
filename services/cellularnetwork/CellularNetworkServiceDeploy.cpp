#include "CellularNetworkServiceDeploy.h"
#include <utils/Logger.h>

namespace service {

static CellularNetworkServiceDeploy* gInstance = nullptr;

CellularNetworkServiceDeploy::CellularNetworkServiceDeploy()
{
    mSIMProvider = base::shm::SIMProvider::instance();
}

CellularNetworkServiceDeploy::~CellularNetworkServiceDeploy()
{
}

CellularNetworkServiceDeploy* CellularNetworkServiceDeploy::instance()
{
    if (gInstance == nullptr)
    {
        gInstance = new CellularNetworkServiceDeploy();
    }
    return gInstance;
}

void CellularNetworkServiceDeploy::responseServiceReady(const std::string& clientName)
{
    std::lock_guard<std::mutex> lock(mMutex);
    mMqSender.startMsq(base::msq::Msq_CelNetwork_RespReady);
    mMqSender.sendMsq(clientName);
}

void CellularNetworkServiceDeploy::responseSync(const std::string& clientName)
{
    {
        std::lock_guard<std::mutex> lock(mMutex);
        bool allowAccess = mSIMProvider->getIsAllowAccess();
        mMqSender.startMsq(base::msq::Msq_CelNetwork_RespAllowAcessUpdated);
        mMqSender.addParam(allowAccess);
        mMqSender.sendMsq(clientName);
    }
    {
        std::lock_guard<std::mutex> lock(mMutex);
        bool cellular = mSIMProvider->getIsCellular();
        mMqSender.startMsq(base::msq::Msq_CelNetwork_RespCellulatrUpdated);
        mMqSender.addParam(cellular);
        mMqSender.sendMsq(clientName);
    }
    {
        std::lock_guard<std::mutex> lock(mMutex);
        bool maxCompa = mSIMProvider->getIsMaxCompatibility();
        mMqSender.startMsq(base::msq::Msq_CelNetwork_RespMaxCompatibilityChanged);
        mMqSender.addParam(maxCompa);
        mMqSender.sendMsq(clientName);
    }
    {
        std::lock_guard<std::mutex> lock(mMutex);
        std::string wifiPassword = mSIMProvider->getWifiPassword();
        mMqSender.startMsq(base::msq::Msq_CelNetwork_RespWifiPasswordUpdated);
        mMqSender.addParam(wifiPassword.c_str());
        mMqSender.sendMsq(clientName);
    }

}

void CellularNetworkServiceDeploy::responseChangeCellularStatus(bool status)
{
    mClientManager.deploy([this, status](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Msq_CelNetwork_RespCellulatrUpdated);
        mMqSender.addParam(status);
        mMqSender.sendMsq(mqName);
    });
}
void CellularNetworkServiceDeploy::responseChangeAllowAccess(bool status)
{
    mClientManager.deploy([this, status](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Msq_CelNetwork_RespAllowAcessUpdated);
        mMqSender.addParam(status);
        mMqSender.sendMsq(mqName);
    });
}
void CellularNetworkServiceDeploy::responseChangeMaxCompatibility(bool status)
{
    mClientManager.deploy([this, status](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Msq_CelNetwork_RespMaxCompatibilityChanged);
        mMqSender.addParam(status);
        mMqSender.sendMsq(mqName);
    });
}

}
