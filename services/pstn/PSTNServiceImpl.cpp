#include "PSTNServiceImpl.h"
#include <utils/Logger.h>

namespace service {

PSTNServiceImpl::PSTNServiceImpl()
    : common::BaseServiceImpl(PSTNServiceDeploy::instance())
{
    mDeploy = PSTNServiceDeploy::instance();
    mSIMDriver = driver::SIMDriver::getInstance();
}

void PSTNServiceImpl::onMsqReceived()
{
    std::vector<std::string> messages = mMqReceiver.receive("/PSTNReq");
    if (!messages.empty()) 
    {
        base::msq::Msq_PSTN_Req type = static_cast<base::msq::Msq_PSTN_Req>(mMqReceiver.get<int>(messages[0]));
        switch (type) {
        case base::msq::Msq_PSTN_ReqRegisterClient: {
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            registerClient(clientName);
            break;
        }
        case base::msq::Msq_PSTN_ReqCallNumber: {
            std::string phoneNumber = mMqReceiver.get<std::string>(messages[1]);
            mSIMDriver->callNumber(phoneNumber);
            break;
        }
        case base::msq::Msq_PSTN_ReqAnswerCall: {
            mSIMDriver->answerCall();
            break;
        }
        case base::msq::Msq_PSTN_ReqRejectCall: {
            mSIMDriver->rejectCall();
            break;
        }
        case base::msq::Msq_PSTN_ReqTerminateCall: {
            mSIMDriver->terminateCall();
            break;
        }
        }
    }
}

void PSTNServiceImpl::initialize()
{
    LOG_INFO("PSTNServiceImpl initialize");

    Connection::connect(mSIMDriver->onDriverReady, std::bind(&PSTNServiceImpl::onSIMDriverReady, this));
    Connection::connect(mSIMDriver->onTimeUpdated, std::bind(&PSTNServiceImpl::onTimeUpdated, this, std::placeholders::_1));
    Connection::connect(mSIMDriver->onCallStatusUpdated, std::bind(&PSTNServiceImpl::onCallStatusUpdated, this, std::placeholders::_1, std::placeholders::_2));
    Connection::connect(mSIMDriver->onCallInfoUpdated, std::bind(&PSTNServiceImpl::onCallInfoUpdated, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    
    mSIMDriver->connectDriver();
}

void PSTNServiceImpl::finialize()
{
    LOG_INFO("PSTNServiceImpl finialize");
}

void PSTNServiceImpl::registerClient(const std::string& clientName)
{
    if (mDeploy->registerClient(clientName))
    {
        mDeploy->responseServiceReady(clientName);
    }
}

void PSTNServiceImpl::onSIMDriverReady()
{
    // To do
}

void PSTNServiceImpl::onTimeUpdated(int time)
{
    mDeploy->responseTimeUpdated(time);
}

void PSTNServiceImpl::onCallStatusUpdated(service::CallStatus callStatus, const std::string& number)
{
    mDeploy->responseCallStatusUpdated(callStatus, number);
}

void PSTNServiceImpl::onCallInfoUpdated(const std::string& number, const std::string& name, const std::string& avatar)
{
    mDeploy->responseCallInfoUpdated(number, name, avatar);
}

}