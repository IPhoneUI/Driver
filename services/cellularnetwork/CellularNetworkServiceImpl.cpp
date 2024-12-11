#include "CellularNetworkServiceImpl.h"
#include <utils/Logger.h>

namespace service {

CellularNetworkServiceImpl::CellularNetworkServiceImpl()
{
    mSIMProvider = base::shm::SIMProvider::instance();
    mDeploy = CellularNetworkServiceDeploy::instance();
}

void CellularNetworkServiceImpl::onMsqReceived()
{
    std::vector<std::string> messages = mMqReceiver.receive("/cellularNetworkReq");
    if (!messages.empty()) 
    {
        base::msq::Msq_CelNetwork_Req type = static_cast<base::msq::Msq_CelNetwork_Req>(mMqReceiver.get<int>(messages[0]));
        switch (type) {
        case base::msq::Msq_CelNetwork_ReqRegisterClient: {
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            registerClient(clientName);
            break;
        }
        case base::msq::Msq_CelNetwork_ReqSync: {
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            mDeploy->responseSync(clientName);
            break;
        }
        case base::msq::Msq_CelNetwork_ReqChangeAllowAcess: {
            bool allowAcess = mMqReceiver.get<bool>(messages[1]);
            requestChangeAllowAccess(allowAcess);
            break;
        }
        case base::msq::Msq_CelNetwork_ReqChangeMaxCompatibility: {
            bool maxCompa = mMqReceiver.get<bool>(messages[1]);
            requestChangeMaxCompatibility(maxCompa);
            break;
        }
        case base::msq::Msq_CelNetwork_ReqChangeCellulatr: {
            bool cellularStatus = mMqReceiver.get<bool>(messages[1]);
            requestChangeCellularStatus(cellularStatus);
            break;
        }
        }
    }
}

void CellularNetworkServiceImpl::initialize()
{
    LOG_INFO("CellularNetworkServiceImpl initialize");
    mSIMProvider->initialize();
}

void CellularNetworkServiceImpl::finialize()
{
    LOG_INFO("CellularNetworkServiceImpl finialize");
    mSIMProvider->closeShmem();
}

void CellularNetworkServiceImpl::registerClient(const std::string& clientName)
{
    if (mDeploy->registerClient(clientName))
    {
        mDeploy->responseDriverReady(clientName);
    }
}

void CellularNetworkServiceImpl::requestChangeCellularStatus(bool status)
{
    auto result = mSIMProvider->setIsCellular(status);

    if (result == base::shm::DataSetResult_Valid)
    {
        mDeploy->responseChangeCellularStatus(status);
    }
}

void CellularNetworkServiceImpl::requestChangeAllowAccess(bool status)
{
    auto result = mSIMProvider->setIsAllowAccess(status);

    if (result == base::shm::DataSetResult_Valid)
    {
        mDeploy->responseChangeAllowAccess(status);
    }
}

void CellularNetworkServiceImpl::requestChangeMaxCompatibility(bool status)
{
    auto result = mSIMProvider->setIsMaxCompatibility(status);

    if (result == base::shm::DataSetResult_Valid)
    {
        mDeploy->responseChangeMaxCompatibility(status);
    }
}

}