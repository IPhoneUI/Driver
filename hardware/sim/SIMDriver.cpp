#include "SIMDriver.h"
#include <utils/Logger.h>

namespace driver {

SIMDriver::SIMDriver()
{

    // mState = Connected;

    // storages::DataRepoManager& dataRepo = storages::DataRepoManager::instance();

    // if (dataRepo.isReady())
    // {
    //     storages::Repository& repo = dataRepo.getRepository("SIM");

    //     std::string number = repo[storages::ParameterIndex::SIM_PhoneNumber];
    //     std::string network = repo[storages::ParameterIndex::SIM_Network];
    //     int phoneSignal = repo[storages::ParameterIndex::SIM_PhoneSignal];
    //     std::string wifiPass = repo[storages::ParameterIndex::SIM_WifiPassword];
    //     bool allowAccess = repo[storages::ParameterIndex::SIM_AllowAccess];
    //     bool cellularSts = repo[storages::ParameterIndex::SIM_CellularStatus];
    //     bool maxCompa = repo[storages::ParameterIndex::SIM_MaxCompatibility];

    //     updatePhoneNumber(number);
    //     updateNetwork(network);
    //     updatePhoneSignal(static_cast<PhoneSignal>(phoneSignal));
    //     updateWifiPassword(wifiPass);
    //     updateAllowAccessStatus(allowAccess);
    //     updateCellularStatus(cellularSts);
    //     updateMaxCompatibility(maxCompa);
    // }

    //     storages::DataRepoManager& dataRepo = storages::DataRepoManager::instance();

    // if (dataRepo.isReady())
    // {
    //     storages::Repository& repo = dataRepo.getRepository("Cloud");

    //     auto contactMap = repo[storages::ParameterIndex::Cloud_Contact].toList();
    //     for (auto it = contactMap.begin(); it != contactMap.end(); ++it)
    //     {
    //         std::unordered_map<std::string, storages::Parameter> item = (*it);
    //         service::PhoneContactInfo contactInfo;
    //         contactInfo.id = 0;
    //         contactInfo.firstName = std::string(item["firstname"]);
    //         contactInfo.lastName = std::string(item["lastname"]);
    //         contactInfo.formatName = std::string(item["formatname"]);
    //         contactInfo.phoneNumber = std::string(item["phonenumber"]);
    //         contactInfo.photo = std::string(item["photo"]);
    //         contactInfo.isFav = bool(item["isfav"]);

    //         mContactData.push_back(contactInfo);
    //     }

    //     auto historyMap = repo[storages::ParameterIndex::Cloud_History].toList();
    //     for (auto it = historyMap.begin(); it != historyMap.end(); ++it)
    //     {
    //         std::unordered_map<std::string, storages::Parameter> item = (*it);
    //         service::PhoneHistoryInfo historyInfo;
    //         historyInfo.id = 0;
    //         historyInfo.formatName = std::string(item["formatName"]);
    //         historyInfo.phoneNumber = std::string(item["phoneNumber"]);
    //         historyInfo.time = std::string(item["time"]);
    //         historyInfo.callingType = static_cast<service::PhoneCallingType>(int(item["phoneType"]));

    //         mHistoryData.push_back(historyInfo);
    //     }

    //     auto recordingMap = repo[storages::ParameterIndex::Cloud_Recording].toList();
    //     for (auto it = recordingMap.begin(); it != recordingMap.end(); ++it)
    //     {
    //         std::unordered_map<std::string, storages::Parameter> item = (*it);
    //         VoiceRecordingData info;
    //         info.id = 0;
    //         info.name = std::string(item["name"]);
    //         info.time = std::string(item["time"]);
    //         info.duration = static_cast<size_t>(int(item["duration"]));

    //         mRecordings.push_back(info);
    //     }

    //     auto deleteRecordingMap = repo[storages::ParameterIndex::Cloud_DeleteRecording].toList();
    //     for (auto it = deleteRecordingMap.begin(); it != deleteRecordingMap.end(); ++it)
    //     {
    //         std::unordered_map<std::string, storages::Parameter> item = (*it);
    //         VoiceRecordingData info;
    //         info.id = 0;
    //         info.name = std::string(item["name"]);
    //         info.time = std::string(item["time"]);
    //         info.duration = static_cast<size_t>(int(item["duration"]));

    //         mDeletedRecording.push_back(info);
    //     }
    // }


    // for (size_t i = 0; i < (size_t)CloudCallbackEvent::Max; ++i)
    // {
    //     mDataQueue.push(i);
    // }
}

void SIMDriver::onMsqReceived()
{
        // if (!mDataQueue.isEmpty())
        // {
        //     StreamingDataType type = static_cast<StreamingDataType>(mDataQueue.get());
        //     LOG_INFO("Data changed: %d [%s]", (int)type, SERVICE_NAME);
        //     for (auto it = mFuncs.begin(); it != mFuncs.end(); ++it)
        //     {
        //         (*it)(static_cast<int>(type));
        //     }

        //     mDataQueue.pop();
        // }
}

void SIMDriver::initialize()
{
    LOG_INFO("SIMDriver initialize");
}

void SIMDriver::finialize()
{
    LOG_INFO("SIMDriver finialize");
}

void SIMDriver::updatePhoneNumber(const std::string& value)
{
    if (mPhoneNumber == value)
        return;
    
    mPhoneNumber = value;
    // mDataQueue.push(static_cast<size_t>(StreamingDataType::PhoneNumber));
}

void SIMDriver::updateNetwork(const std::string& value)
{
    if (mNetwork == value)
        return;
    
    mNetwork = value;
    // mDataQueue.push(static_cast<size_t>(StreamingDataType::Network));
}

void SIMDriver::updatePhoneSignal(const PhoneSignal& value)
{
    // if (mPhoneSignal == value)
    //     return;
    
    // mPhoneSignal = value;
    // // mDataQueue.push(static_cast<size_t>(StreamingDataType::PhoneSignal));
}

void SIMDriver::updateCellularStatus(const bool& status)
{
    if (mCellular == status)
        return;
    
    mCellular = status;
    // mDataQueue.push(static_cast<size_t>(StreamingDataType::CellularStatus));
}

void SIMDriver::updateAllowAccessStatus(const bool& status)
{
    if (mAllowAccess == status)
        return;
    
    mAllowAccess = status;
    // mDataQueue.push(static_cast<size_t>(StreamingDataType::AllowAccess));
}

void SIMDriver::updateMaxCompatibility(const bool& status)
{
    if (mMaxCompatibility == status)
        return;
    
    mMaxCompatibility = status;
    // mDataQueue.push(static_cast<size_t>(StreamingDataType::MaxCompatibility));
}

void SIMDriver::updateWifiPassword(const std::string& value)
{
    if (mWifiPassword == value)
        return;

    mWifiPassword = value;
    // mDataQueue.push(static_cast<size_t>(StreamingDataType::WifiPassord));
}

}