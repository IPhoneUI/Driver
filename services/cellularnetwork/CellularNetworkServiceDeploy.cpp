#include "CellularNetworkServiceDeploy.h"
#include <utils/Logger.h>

namespace service {

static CellularNetworkServiceDeploy* gInstance = nullptr;

CellularNetworkServiceDeploy::CellularNetworkServiceDeploy()
{
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
