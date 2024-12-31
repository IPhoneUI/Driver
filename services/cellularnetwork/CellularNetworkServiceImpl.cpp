#include "CellularNetworkServiceImpl.h"
#include <utils/Logger.h>

namespace service {

CellularNetworkServiceImpl::CellularNetworkServiceImpl()
    : common::BaseServiceImpl(CellularNetworkServiceDeploy::instance())
{
    mDeploy = CellularNetworkServiceDeploy::instance();
    mSIMDriver = driver::SIMDriver::getInstance();
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
            requestSync();
            break;
        }
        case base::msq::Msq_CelNetwork_ReqChangeAllowAcess: {
            bool allowAcess = mMqReceiver.get<bool>(messages[1]);
            mSIMDriver->changeAllowAccess(allowAcess);
            break;
        }
        case base::msq::Msq_CelNetwork_ReqChangeMaxCompatibility: {
            bool maxCompa = mMqReceiver.get<bool>(messages[1]);
            mSIMDriver->changeMaxCompatibility(maxCompa);
            break;
        }
        case base::msq::Msq_CelNetwork_ReqChangeCellulatr: {
            bool cellularStatus = mMqReceiver.get<bool>(messages[1]);
            mSIMDriver->changeCellularStatus(cellularStatus);
            break;
        }
        }
    }
}

void CellularNetworkServiceImpl::initialize()
{
    LOG_INFO("CellularNetworkServiceImpl initialize");
    Connection::connect(mSIMDriver->onDriverReady, std::bind(&CellularNetworkServiceImpl::onSIMDriverReady, this));
    Connection::connect(mSIMDriver->onAllowAccessUpdated, std::bind(&CellularNetworkServiceImpl::onAllowAccessUpdated, this, std::placeholders::_1));
    Connection::connect(mSIMDriver->onCellularStatusUpdated, std::bind(&CellularNetworkServiceImpl::onCellularStatusUpdated, this, std::placeholders::_1));
    Connection::connect(mSIMDriver->onMaxCompatibilityUpdated, std::bind(&CellularNetworkServiceImpl::onMaxCompatibilityUpdated, this, std::placeholders::_1));

    mSIMDriver->connectDriver();
}

void CellularNetworkServiceImpl::finialize()
{
    LOG_INFO("CellularNetworkServiceImpl finialize");
}

void CellularNetworkServiceImpl::requestSync()
{
    {
        bool allowAccess = mSIMDriver->getAllowAccess();
        mDeploy->responseChangeAllowAccess(allowAccess);

        bool cellularSts = mSIMDriver->getCellularStatus();
        mDeploy->responseChangeCellularStatus(cellularSts);

        bool maxCompa = mSIMDriver->getMaxCompatibitily();
        mDeploy->responseChangeMaxCompatibility(maxCompa);
    }

}

void CellularNetworkServiceImpl::onSIMDriverReady()
{
    // To do
}

void CellularNetworkServiceImpl::onCellularStatusUpdated(bool status)
{
    mDeploy->responseChangeCellularStatus(status);
}

void CellularNetworkServiceImpl::onAllowAccessUpdated(bool status)
{
    mDeploy->responseChangeAllowAccess(status);
}

void CellularNetworkServiceImpl::onMaxCompatibilityUpdated(bool status)
{
    mDeploy->responseChangeMaxCompatibility(status);
}

}