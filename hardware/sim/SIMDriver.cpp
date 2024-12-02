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

    mProvider = SIMProvider::instance();
    mDeploy = SIMDeploy::instance();
}

void SIMDriver::onMsqReceived()
{
    std::vector<std::string> messages = mMqReceiver.receive("/SIMReq");
    if (!messages.empty()) 
    {
        service::Msq_SIMReq type = static_cast<service::Msq_SIMReq>(mMqReceiver.get<int>(messages[0]));
        switch (type) {
        case service::SIM_CelNetwork_RegisterClient: {
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            registerClient(service::Msq_CelNetwork_Client, clientName);
            break;
        }
        case service::SIM_CelNetwork_ReqSync: {
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            requestSync(type, clientName);
            break;
        }
        case service::SIM_CelNetwork_ReqChangeAllowAcess: {
            bool allowAcess = mMqReceiver.get<bool>(messages[2]);
            requestChangeAllowAccess(allowAcess);
            break;
        }
        case service::SIM_CelNetwork_ReqChangeMaxCompatibility: {
            bool maxCompa = mMqReceiver.get<bool>(messages[2]);
            requestChangeMaxCompatibility(maxCompa);
            break;
        }
        case service::SIM_CelNetwork_ReqChangeCellulatr: {
            bool cellularStatus = mMqReceiver.get<bool>(messages[2]);
            requestChangeCellularStatus(cellularStatus);
            break;
        }
        case service::SIM_PhoneBook_RegisterClient: {
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            registerClient(service::Msq_PhoneBook_Client, clientName);
            break;
        }
        case service::SIM_PhoneBook_ReqSync: {
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            requestSync(type, clientName);
            break;
        }
        }
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

void SIMDriver::registerClient(service::Msq_Client clientId, const std::string& clientName)
{
    if (mDeploy->registerClient(clientId, clientName))
    {
        mDeploy->responseDriverReady(clientName);
    }
}

void SIMDriver::requestSync(service::Msq_SIMReq type, const std::string& clientName)
{
    if (type == service::SIM_PhoneBook_ReqSync)
    {
        mDeploy->responseSyncPhoneBook(clientName);
    }
    else if (type == service::SIM_CelNetwork_ReqSync)
    {
        mDeploy->responseSyncCelNetwork(clientName);
    }
}

void SIMDriver::requestChangeCellularStatus(bool status)
{
    auto result = mProvider->setIsCellular(status);

    if (result == DataSetResult_Valid)
    {
        mDeploy->responseChangeCellularStatus(status);
    }
}

void SIMDriver::requestChangeAllowAccess(bool status)
{
    auto result = mProvider->setIsAllowAccess(status);

    if (result == DataSetResult_Valid)
    {
        mDeploy->responseChangeAllowAccess(status);
    }
}

void SIMDriver::requestChangeMaxCompatibility(bool status)
{
    auto result = mProvider->setIsMaxCompatibility(status);

    if (result == DataSetResult_Valid)
    {
        mDeploy->responseChangeMaxCompatibility(status);
    }
}

}