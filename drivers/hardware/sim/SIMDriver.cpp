#include "SIMDriver.h"
#include <utils/Logger.h>
#include <DriverExecution.h>

namespace driver {

static SIMDriver* gInstance = 0;

SIMDriver::SIMDriver()
{
    common::DriverExecution::instance().addDriver("SIMDriver", this);
    readDataFromDatabase();
}

SIMDriver* SIMDriver::getInstance()
{
    if (gInstance == nullptr)
    {
        gInstance = new SIMDriver();
    }

    return gInstance;
}

void SIMDriver::initialize()
{
    LOG_INFO("SIMDriver initialize");
    if (gInstance == nullptr)
    {
        gInstance = new SIMDriver();
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
            onCallStatusUpdated.emit(mCallInfo->status, mCallInfo->number);
            break;
        case PSTNEvent::CallInfoUpdated:
            if (mCallInfo == nullptr)
                return;

            onCallInfoUpdated.emit(mCallInfo->number, mCallInfo->name, mCallInfo->avatar);
            break;
        case PSTNEvent::TimeUpdated:
            if (mCallInfo == nullptr)
                return;
                
            onTimeUpdated.emit(mCallInfo->time);
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

void SIMDriver::connectDriver()
{
    mIsReady = true;
    onDriverReady.emit();
}

void SIMDriver::readDataFromDatabase()
{
    common::DataRepoManager& dataRepo = common::DataRepoManager::instance();

    if (dataRepo.isReady())
    {
        common::Repository& repo = dataRepo.getRepository("sim");

        std::string number = repo[common::ParameterIndex::SIM_PhoneNumber];
        mPhoneNumber = number;

        std::string network = repo[common::ParameterIndex::SIM_Network];
        mNetwork = network;

        int phoneSignal = repo[common::ParameterIndex::SIM_PhoneSignal];
        mPhoneSignal = phoneSignal;

        std::string wifiPass = repo[common::ParameterIndex::SIM_WifiPassword];
        mWifiPassword = wifiPass;
        
        bool allowAccess = repo[common::ParameterIndex::SIM_AllowAccess];
        mAllowAccess = allowAccess;

        bool cellularSts = repo[common::ParameterIndex::SIM_CellularStatus];
        mCellularSts = cellularSts;

        bool maxCompa = repo[common::ParameterIndex::SIM_MaxCompatibility];
        mMaxCompatibility = maxCompa;

        auto contactMap = repo[common::ParameterIndex::SIM_Contact].toList();
        int count = 0;
        for (auto it = contactMap.begin(); it != contactMap.end(); ++it)
        {
            std::unordered_map<std::string, common::Parameter> item = (*it);
            mContacts.push_back({
                static_cast<uint32_t>(count),
                std::string(item["firstname"]),
                std::string(item["lastname"]),
                std::string(item["formatname"]),
                std::string(item["phonenumber"]),
                std::string(item["photo"]),
                bool(item["isfav"])
            });
            ++count;
        }

        count = 0;
        auto historyMap = repo[common::ParameterIndex::SIM_History].toList();
        for (auto it = historyMap.begin(); it != historyMap.end(); ++it)
        {
            std::unordered_map<std::string, common::Parameter> item = (*it);
            mHistories.push_back({
                static_cast<uint32_t>(count),
                std::string(item["formatName"]),
                std::string(item["phoneNumber"]),
                std::string(item["time"]),
                static_cast<service::PhoneCallingType>(int(item["phoneType"]))
            });
            ++count;
        }
    }
}

void SIMDriver::requestSync(ServiceReq type)
{
    if (type == PhoneBookService)
    {
        onPhoneContactListUpdated.emit();
        onPhoneHistoryListUpdated.emit();
    }
}

// void SIMDriver::requestChangeCellularStatus(bool status)
// {
//     auto result = mProvider->setIsCellular(status);

//     if (result == base::shm::DataSetResult_Valid)
//     {
//         mDeploy->responseChangeCellularStatus(status);
//     }
// }

// void SIMDriver::requestChangeAllowAccess(bool status)
// {
//     auto result = mProvider->setIsAllowAccess(status);

//     if (result == base::shm::DataSetResult_Valid)
//     {
//         mDeploy->responseChangeAllowAccess(status);
//     }
// }

// void SIMDriver::requestChangeMaxCompatibility(bool status)
// {
//     auto result = mProvider->setIsMaxCompatibility(status);

//     if (result == base::shm::DataSetResult_Valid)
//     {
//         mDeploy->responseChangeMaxCompatibility(status);
//     }
// }
//
void SIMDriver::callNumber(const std::string& number)
{
    if (mCallInfo != nullptr)
        return;

    mCallInfo = new service::CallInformation();
    mCallInfo->status = service::CallStatus::Outgoing;
    mCallInfo->number = number;

    for (auto it = mContacts.begin(); it != mContacts.end(); ++it)
    {
        if ((*it).phoneNumber == number)
        {
            mCallInfo->name = std::string((*it).formatName);
            mCallInfo->avatar = std::string((*it).photo);
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