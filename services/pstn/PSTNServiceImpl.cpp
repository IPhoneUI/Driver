#include "PSTNServiceImpl.h"
#include <utils/Logger.h>

namespace service {

PSTNServiceImpl::PSTNServiceImpl()
{
    mSIMProvider = base::shm::SIMProvider::instance();
    mDeploy = PSTNServiceDeploy::instance();
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
            callNumber(phoneNumber);
            break;
        }
        case base::msq::Msq_PSTN_ReqAnswerCall: {
            answerCall();
            break;
        }
        case base::msq::Msq_PSTN_ReqRejectCall: {
            rejectCall();
            break;
        }
        case base::msq::Msq_PSTN_ReqTerminateCall: {
            terminateCall();
            break;
        }
        }
    }
}

void PSTNServiceImpl::initialize()
{
    LOG_INFO("PSTNServiceImpl initialize");
    mSIMProvider->initialize();
}

void PSTNServiceImpl::finialize()
{
    LOG_INFO("PSTNServiceImpl finialize");
    mSIMProvider->closeShmem();
}

void PSTNServiceImpl::execute(milliseconds delta)
{
    static milliseconds checkActiveState = milliseconds(0); 

    if (!mEventQueue.isEmpty())
    {
        PSTNEvent event = mEventQueue.front();

        switch(static_cast<PSTNEvent>(event)) {
        case PSTNEvent::CallStatusUpdated:
            if (mCallInfo == nullptr)
                return;
            mDeploy->responseCallStatusUpdated(mCallInfo->status, mCallInfo->number);
            break;
        case PSTNEvent::CallInfoUpdated:
            if (mCallInfo == nullptr)
                return;

            mDeploy->responseCallInfoUpdated(mCallInfo->number, mCallInfo->name, mCallInfo->avatar);
            break;
        case PSTNEvent::TimeUpdated:
            if (mCallInfo == nullptr)
                return;

            mDeploy->responseTimeUpdated(mCallInfo->time);
            break;
        default:
            break;
        }

        mEventQueue.pop();
    }

    if (mCallInfo != nullptr && (mCallInfo->status == service::CallStatus::Incoming || mCallInfo->status == service::CallStatus::Outgoing))
    {
        checkActiveState += delta;
        if (checkActiveState > milliseconds(5000))
        {
            checkActiveState = milliseconds(0);
            mCallInfo->status = service::CallStatus::Active;
            mEventQueue.push(PSTNEvent::CallStatusUpdated);

        }
    }

    updateTimeCall(delta);

    if (mCallInfo != nullptr && mCallInfo->status == service::CallStatus::Idle)
    {
        delete mCallInfo;
        mCallInfo = nullptr;
    }
}

void PSTNServiceImpl::registerClient(const std::string& clientName)
{
    if (mDeploy->registerClient(clientName))
    {
        mDeploy->responseDriverReady(clientName);
    }
}

void PSTNServiceImpl::callNumber(const std::string& number)
{
    if (mCallInfo != nullptr)
        return;

    mCallInfo = new service::CallInformation();
    mCallInfo->status = service::CallStatus::Outgoing;
    mCallInfo->number = number;

    auto contactList = mSIMProvider->getPhoneContactList();

    for (auto it = contactList.begin(); it != contactList.end(); ++it)
    {
        if (std::strcmp(it->phoneNumber, number.c_str()) == 0)
        {
            mCallInfo->name = std::string(it->formatName);
            mCallInfo->avatar = std::string(it->photo);
            break;
        }
    }

    mEventQueue.push(PSTNEvent::CallStatusUpdated);
    mEventQueue.push(PSTNEvent::CallInfoUpdated);
}

void PSTNServiceImpl::answerCall()
{
    if (mCallInfo == nullptr)
        return;
    
    if (mCallInfo->status == service::CallStatus::Outgoing 
        || mCallInfo->status == service::CallStatus::Incoming)
    {
        mCallInfo->status = service::CallStatus::Active;
        mEventQueue.push(PSTNEvent::CallStatusUpdated);
    }
}

void PSTNServiceImpl::rejectCall()
{
    if (mCallInfo == nullptr)
        return;
    
    if (mCallInfo->status == service::CallStatus::Incoming)
    {
        mCallInfo->status = service::CallStatus::Idle;
        mEventQueue.push(PSTNEvent::CallStatusUpdated);
    }
}

void PSTNServiceImpl::terminateCall()
{
    if (mCallInfo == nullptr)
        return;
    
    if (mCallInfo->status == service::CallStatus::Active 
        || mCallInfo->status == service::CallStatus::Outgoing)
    {
        mCallInfo->status = service::CallStatus::Idle;
        mEventQueue.push(PSTNEvent::CallStatusUpdated);
    }
}

void PSTNServiceImpl::updateTimeCall(milliseconds delta)
{
    static milliseconds sTime {milliseconds(0)};

    if (mCallInfo != nullptr && mCallInfo->status == service::CallStatus::Active)
    {
        sTime += delta;
        if (sTime > milliseconds(1000))
        {
            mCallInfo->time += 1000;
            mEventQueue.push(PSTNEvent::TimeUpdated);
            sTime = milliseconds(0);
        }
    }
    else {
        sTime = milliseconds(0);
    }
}

}