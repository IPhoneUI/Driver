#include "SIMDriver.h"
#include <utils/Logger.h>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace driver {

static SIMDriver* gInstance = 0;

SIMDriver::SIMDriver()
{
    mRepo.setName("sim");
    mRepo.addParam("phonenumber", common::ParameterIndex::SIM_PhoneNumber);
    mRepo.addParam("network", common::ParameterIndex::SIM_Network);
    mRepo.addParam("phonesignal", common::ParameterIndex::SIM_PhoneSignal);
    mRepo.addParam("wifipassword", common::ParameterIndex::SIM_WifiPassword);
    mRepo.addParam("allowaccess", common::ParameterIndex::SIM_AllowAccess);
    mRepo.addParam("cellular", common::ParameterIndex::SIM_CellularStatus);
    mRepo.addParam("maxcompatibility", common::ParameterIndex::SIM_MaxCompatibility);
    mRepo.addParam("contact", common::ParameterIndex::SIM_Contact);
    mRepo.addParam("history", common::ParameterIndex::SIM_History);

    Connection::connect(mRepo.onRepoStateChanged, std::bind(&SIMDriver::onRepoStateChanged, this, std::placeholders::_1));

    mRepo.pull();

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

    updateTimeCall(delta);

    if (mCallInfo != nullptr && mCallInfo->status == service::CallStatus::Idle)
    {
        std::time_t now = std::time(nullptr);
        std::tm* localTime = std::localtime(&now);

        std::ostringstream oss;
        oss << std::put_time(localTime, "%Y%m%d%H%M%S");
        std::string dateTime = oss.str();

        mHistories.emplace_back(
            static_cast<uint32_t>(mHistories.size()),
            mCallInfo->name,
            mCallInfo->number,
            dateTime,
            mCallInfo->callType
        );
        onPhoneHistoryListUpdated.emit();
        delete mCallInfo;
        mCallInfo = nullptr;
    }

    if (mCallInfo != nullptr && (mCallInfo->status == service::CallStatus::Incoming || mCallInfo->status == service::CallStatus::Outgoing))
    {
        checkActiveState += delta;
        if (checkActiveState > milliseconds(10000))
        {
            checkActiveState = milliseconds(0);
            mCallInfo->callType = service::PhoneCallingType::Missed;
            mCallInfo->status = service::CallStatus::Idle;
            mEventQueue.push(PSTNEvent::CallStatusUpdated);
        }
    }
}

void SIMDriver::connectDriver()
{
    mIsReady = true;
    onDriverReady.emit();
}

void SIMDriver::writeBuffer()
{
    mRepo[common::ParameterIndex::SIM_PhoneNumber] = mPhoneNumber;
    mRepo[common::ParameterIndex::SIM_Network] = mNetwork;
    mRepo[common::ParameterIndex::SIM_PhoneSignal] = mPhoneSignal;
    mRepo[common::ParameterIndex::SIM_WifiPassword] = mWifiPassword;        
    mRepo[common::ParameterIndex::SIM_AllowAccess] = mAllowAccess;
    mRepo[common::ParameterIndex::SIM_CellularStatus] = mCellularSts;
    mRepo[common::ParameterIndex::SIM_MaxCompatibility] = mMaxCompatibility;

    utils::VariantList contactList;
    for (const auto& contactItem : mContacts)
    {
        std::unordered_map<std::string, utils::Variant> item;
        item["firstname"] = contactItem.firstName;
        item["lastname"] = contactItem.lastName;
        item["formatname"] = contactItem.formatName;
        item["phonenumber"] = contactItem.phoneNumber;
        item["photo"] = contactItem.photo;
        item["isfav"] = contactItem.isFav;

        contactList.push(item);
    }

    utils::VariantList historyList;
    for (const auto& historyItem : mHistories)
    {
        std::unordered_map<std::string, utils::Variant> item;
        item["formatName"] = historyItem.formatName;
        item["phoneNumber"] = historyItem.phoneNumber;
        item["time"] = historyItem.time;
        item["phoneType"] = static_cast<int>(historyItem.callingType);

        historyList.push(item);
    }

    mRepo[common::ParameterIndex::SIM_Contact] = contactList;
    mRepo[common::ParameterIndex::SIM_History] = historyList;
    
    mRepo.push();
}

void SIMDriver::onSimulateReceived(const std::string& topic, const std::string& option, const std::string& content)
{
    if (topic == "Phone Calling")
    {
        if (option == "Answer Call")
        {
            if (mCallInfo == nullptr)
                return;

            if (mCallInfo != nullptr && (mCallInfo->status == service::CallStatus::Incoming 
                || mCallInfo->status == service::CallStatus::Outgoing))
            {
                mCallInfo->status = service::CallStatus::Active;
                mEventQueue.push(PSTNEvent::CallStatusUpdated);
            }
        }
        else if (option == "Incoming Call")
        {
            if (mCallInfo != nullptr)
                return;

            mCallInfo = new service::CallInformation();
            mCallInfo->status = service::CallStatus::Incoming;
            mCallInfo->number = content;
            mCallInfo->callType = service::PhoneCallingType::Incoming;

            for (auto it = mContacts.begin(); it != mContacts.end(); ++it)
            {
                if (strcmp((*it).phoneNumber.c_str(), content.c_str()) == 0)
                {
                    mCallInfo->name = std::string((*it).formatName);
                    mCallInfo->avatar = std::string((*it).photo);
                    break;
                }
            }

            mEventQueue.push(PSTNEvent::CallStatusUpdated);
            mEventQueue.push(PSTNEvent::CallInfoUpdated);
        }
    }
}

void SIMDriver::onRepoStateChanged(common::Repository::State state)
{
    if (state == common::Repository::PullCompleted)
    {
        mPhoneNumber = mRepo[common::ParameterIndex::SIM_PhoneNumber];
        mNetwork = mRepo[common::ParameterIndex::SIM_Network];
        mPhoneSignal = mRepo[common::ParameterIndex::SIM_PhoneSignal];
        mWifiPassword = mRepo[common::ParameterIndex::SIM_WifiPassword];        
        mAllowAccess = mRepo[common::ParameterIndex::SIM_AllowAccess];
        mCellularSts = mRepo[common::ParameterIndex::SIM_CellularStatus];
        mMaxCompatibility = mRepo[common::ParameterIndex::SIM_MaxCompatibility];

        utils::VariantList contactList = mRepo[common::ParameterIndex::SIM_Contact];
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
        utils::VariantList historyList = mRepo[common::ParameterIndex::SIM_History];
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
    if (!mCellularSts)
    {
        changeAllowAccess(false);
    }
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
    mCallInfo->callType = service::PhoneCallingType::Outgoing;

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