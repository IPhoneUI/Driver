#include "FlashMemoryDriver.h"
#include <utils/Logger.h>

namespace driver {

static FlashMemoryDriver* gInstance = 0;

FlashMemoryDriver::FlashMemoryDriver()
{
    common::DataRepoManager& dataRepo = common::DataRepoManager::instance();
    auto flashmemoryRepo = dataRepo.addRepository("flashmemory");
    if (flashmemoryRepo != nullptr)
    {
        flashmemoryRepo->addParameter("recording", common::ParameterIndex::FMem_Recording);
        flashmemoryRepo->addParameter("delete_recording", common::ParameterIndex::FMem_DeleteRecording);
        flashmemoryRepo->addParameter("airplane_mode", common::ParameterIndex::FMem_AirPlaneMode);

        Connection::connect(flashmemoryRepo->onRepoStateChanged, std::bind(&FlashMemoryDriver::onRepoStateChanged, this, std::placeholders::_1));
    }

    common::DriverExecution::instance().addDriver("FlashMemoryDriver", this);
}

FlashMemoryDriver* FlashMemoryDriver::getInstance()
{
    if (gInstance == nullptr)
    {
        gInstance = new FlashMemoryDriver();
    }

    return gInstance;
}

void FlashMemoryDriver::connectDriver()
{
    mIsReady = true;
    onDriverReady.emit();
}

void FlashMemoryDriver::readDataFromDatabase()
{
}

void FlashMemoryDriver::onRepoStateChanged(common::Repository::State state)
{
    if (state == common::Repository::PullCompleted)
    {
        common::DataRepoManager& dataRepo = common::DataRepoManager::instance();
        common::Repository& repo = dataRepo.repository("flashmemory");

        auto recordingMap = repo[common::ParameterIndex::FMem_Recording].toList();
        for (auto it = recordingMap.begin(); it != recordingMap.end(); ++it)
        {
            std::unordered_map<std::string, common::PTree> item = (*it);
            service::VoiceRecordingData* data = new service::VoiceRecordingData();
            data->name = std::string(item["name"]);
            data->time = std::string(item["time"]);
            data->duration = item["duration"];
            mRecordingData.push_back(data);     
        }

        auto deleteRecordingMap = repo[common::ParameterIndex::FMem_DeleteRecording].toList();
        for (auto it = deleteRecordingMap.begin(); it != deleteRecordingMap.end(); ++it)
        {
            std::unordered_map<std::string, common::PTree> item = (*it);
            service::VoiceRecordingData* data = new service::VoiceRecordingData();
            data->name = std::string(item["name"]);
            data->time = std::string(item["time"]);
            data->duration = item["duration"];
            mDeleteRecordingData.push_back(data);       
        }
        
        mAirPlaneMode = repo[common::ParameterIndex::FMem_AirPlaneMode];
    }
}

void FlashMemoryDriver::changeAirPlaneMode(bool airPlane)
{
    if (mAirPlaneMode == airPlane)
        return; 
    
    mAirPlaneMode = airPlane;
    onAirPlaneModeUpdated.emit(mAirPlaneMode);
}

}