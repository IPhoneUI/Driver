#include "SIMDriver.h"
#include <utils/Logger.h>

namespace driver {

static SIMDriver* gInstance = 0;

SIMDriver::SIMDriver()
{
    common::DataRepoManager& dataRepo = common::DataRepoManager::instance();
    auto repoPtr = dataRepo.addRepository("sim");
    if (repoPtr)
    {
        repoPtr->addParameter("phonenumber", common::ParameterIndex::SIM_PhoneNumber);
        repoPtr->addParameter("network", common::ParameterIndex::SIM_Network);
        repoPtr->addParameter("phonesignal", common::ParameterIndex::SIM_PhoneSignal);
        repoPtr->addParameter("wifipassword", common::ParameterIndex::SIM_WifiPassword);
        repoPtr->addParameter("allowaccess", common::ParameterIndex::SIM_AllowAccess);
        repoPtr->addParameter("cellular", common::ParameterIndex::SIM_CellularStatus);
        repoPtr->addParameter("maxcompatibility", common::ParameterIndex::SIM_MaxCompatibility);
        repoPtr->addParameter("contact", common::ParameterIndex::SIM_Contact);
        repoPtr->addParameter("history", common::ParameterIndex::SIM_History);

        Connection::connect(repoPtr->onRepoStateChanged, std::bind(&SIMDriver::onRepoStateChanged, this, std::placeholders::_1));
    }

    common::DriverExecution::instance().addDriver("SIMDriver", this);
}

SIMDriver* SIMDriver::getInstance()
{
    if (gInstance == nullptr)
    {
        throw std::runtime_error("The SIMDriver has not been initialized yet");
    }

    return gInstance;
}

void SIMDriver::initialize()
{
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

void SIMDriver::onRepoStateChanged(common::Repository::State state)
{
    LOG_INFO("State changed: %d", (int)state);

    if (state == common::Repository::PullCompleted)
    {
        common::DataRepoManager& dataRepo = common::DataRepoManager::instance();
        common::Repository& repo = dataRepo.repository("sim");

        mPhoneNumber = repo[common::ParameterIndex::SIM_PhoneNumber];
        mNetwork = repo[common::ParameterIndex::SIM_Network];
        mPhoneSignal = repo[common::ParameterIndex::SIM_PhoneSignal];
        mWifiPassword = repo[common::ParameterIndex::SIM_WifiPassword];        
        mAllowAccess = repo[common::ParameterIndex::SIM_AllowAccess];
        mCellularSts = repo[common::ParameterIndex::SIM_CellularStatus];
        mMaxCompatibility = repo[common::ParameterIndex::SIM_MaxCompatibility];

        utils::VariantList contactList = repo[common::ParameterIndex::SIM_Contact];
        int count = 0;
        for (const auto &contact : contactList)
        {
            std::unordered_map<std::string, utils::Variant> item = contact;
            mContacts.push_back({
                static_cast<uint32_t>(count),
                item["firstname"],
                item["lastname"],
                item["formatname"],
                item["phonenumber"],
                item["photo"],
                item["isfav"]
            });
            ++count;
        }

        count = 0;
        utils::VariantList historyList = repo[common::ParameterIndex::SIM_History];
        for (const auto &history : historyList)
        {
            std::unordered_map<std::string, utils::Variant> item = history;
            mHistories.push_back({
                static_cast<uint32_t>(count),
                item["formatName"],
                item["phoneNumber"],
                item["time"],
                static_cast<service::PhoneCallingType>(int(item["phoneType"]))
            });
            ++count;
        }
    }
}

void SIMDriver::changeCellularStatus(bool status)
{
    if (mCellularSts == status)
        return;
    
    mCellularSts = status;
    onCellularStatusUpdated.emit(mCellularSts);
}

void SIMDriver::changeAllowAccess(bool status)
{
    if (mAllowAccess == status)
        return;
    
    mAllowAccess = status;
    onAllowAccessUpdated.emit(mAllowAccess);
}

void SIMDriver::changeMaxCompatibility(bool status)
{
    if (mMaxCompatibility == status)
        return;
    
    mMaxCompatibility = status;
    onMaxCompatibilityUpdated.emit(mMaxCompatibility);
}

void SIMDriver::callNumber(const std::string& number)
{
    if (mCallInfo != nullptr)
        return;

    mCallInfo = new service::CallInformation();
    mCallInfo->status = service::CallStatus::Outgoing;
    mCallInfo->number = number;

    for (auto it = mContacts.begin(); it != mContacts.end(); ++it)
    {
        if (strcmp((*it).phoneNumber.c_str(), number.c_str()) == 0)
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