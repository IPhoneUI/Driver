#include "SIMDriver.h"
#include <utils/Logger.h>

namespace driver {

SIMDriver::SIMDriver()
{
    mProvider = base::shm::SIMProvider::instance();
    mDeploy = SIMDeploy::instance();
}

void SIMDriver::onMsqReceived()
{
    std::vector<std::string> messages = mMqReceiver.receive("/SIMReq");
    if (!messages.empty()) 
    {
        base::msq::Msq_SIMReq type = static_cast<base::msq::Msq_SIMReq>(mMqReceiver.get<int>(messages[0]));
        switch (type) {
        case base::msq::SIM_CelNetwork_RegisterClient: {
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            registerClient(base::msq::Msq_CelNetwork_Client, clientName);
            break;
        }
        case base::msq::SIM_CelNetwork_ReqSync: 
        case base::msq::SIM_PhoneBook_ReqSync:{
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            requestSync(type, clientName);
            break;
        }
        case base::msq::SIM_CelNetwork_ReqChangeAllowAcess: {
            bool allowAcess = mMqReceiver.get<bool>(messages[2]);
            requestChangeAllowAccess(allowAcess);
            break;
        }
        case base::msq::SIM_CelNetwork_ReqChangeMaxCompatibility: {
            bool maxCompa = mMqReceiver.get<bool>(messages[2]);
            requestChangeMaxCompatibility(maxCompa);
            break;
        }
        case base::msq::SIM_CelNetwork_ReqChangeCellulatr: {
            bool cellularStatus = mMqReceiver.get<bool>(messages[2]);
            requestChangeCellularStatus(cellularStatus);
            break;
        }
        case base::msq::SIM_PhoneBook_RegisterClient: {
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            registerClient(base::msq::Msq_PhoneBook_Client, clientName);
            break;
        }
        case base::msq::SIM_PSTN_RegisterClient: {
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            registerClient(base::msq::Msq_PSTN_Client, clientName);
            break;
        }
        case base::msq::SIM_PSTN_ReqCallNumber: {
            std::string phoneNumber = mMqReceiver.get<std::string>(messages[2]);
            callNumber(phoneNumber);
            break;
        }
        case base::msq::SIM_PSTN_ReqAnswerCall: {
            answerCall();
            break;
        }
        case base::msq::SIM_PSTN_ReqRejectCall: {
            rejectCall();
            break;
        }
        case base::msq::SIM_PSTN_ReqTerminateCall: {
            terminateCall();
            break;
        }
        }
    }
}

void SIMDriver::execute(milliseconds delta)
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

void SIMDriver::initialize()
{
    LOG_INFO("SIMDriver initialize");
    mProvider->initialize();
}

void SIMDriver::finialize()
{
    LOG_INFO("SIMDriver finialize");
    mProvider->closeShmem();
}

void SIMDriver::registerClient(base::msq::Msq_Client clientId, const std::string& clientName)
{
    // if (mDeploy->registerClient(clientId, clientName))
    // {
    //     mDeploy->responseDriverReady(clientName);
    // }
}

void SIMDriver::requestSync(base::msq::Msq_SIMReq type, const std::string& clientName)
{
    if (type == base::msq::SIM_PhoneBook_ReqSync)
    {
        mDeploy->responseSyncPhoneBook(clientName);
    }
    else if (type == base::msq::SIM_CelNetwork_ReqSync)
    {
        mDeploy->responseSyncCelNetwork(clientName);
    }
}

void SIMDriver::requestChangeCellularStatus(bool status)
{
    auto result = mProvider->setIsCellular(status);

    if (result == base::shm::DataSetResult_Valid)
    {
        mDeploy->responseChangeCellularStatus(status);
    }
}

void SIMDriver::requestChangeAllowAccess(bool status)
{
    auto result = mProvider->setIsAllowAccess(status);

    if (result == base::shm::DataSetResult_Valid)
    {
        mDeploy->responseChangeAllowAccess(status);
    }
}

void SIMDriver::requestChangeMaxCompatibility(bool status)
{
    auto result = mProvider->setIsMaxCompatibility(status);

    if (result == base::shm::DataSetResult_Valid)
    {
        mDeploy->responseChangeMaxCompatibility(status);
    }
}

void SIMDriver::callNumber(const std::string& number)
{
    if (mCallInfo != nullptr)
        return;

    mCallInfo = new service::CallInformation();
    mCallInfo->status = service::CallStatus::Outgoing;
    mCallInfo->number = number;

    auto contactList = mProvider->getPhoneContactList();

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

void SIMDriver::answerCall()
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

void SIMDriver::rejectCall()
{
    if (mCallInfo == nullptr)
        return;
    
    if (mCallInfo->status == service::CallStatus::Incoming)
    {
        mCallInfo->status = service::CallStatus::Idle;
        mEventQueue.push(PSTNEvent::CallStatusUpdated);
    }
}

void SIMDriver::terminateCall()
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

void SIMDriver::updateTimeCall(milliseconds delta)
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