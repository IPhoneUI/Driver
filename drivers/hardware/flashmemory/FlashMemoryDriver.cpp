#include "FlashMemoryDriver.h"
#include <utils/Logger.h>

namespace driver {

static FlashMemoryDriver* gInstance = 0;

FlashMemoryDriver::FlashMemoryDriver()
{
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
    common::DataRepoManager& dataRepo = common::DataRepoManager::instance();
    if (dataRepo.isReady())
    {
        common::Repository& repo = dataRepo.getRepository("flashmemory");
        auto recordingMap = repo[common::ParameterIndex::FMem_Recording].toList();
        for (auto it = recordingMap.begin(); it != recordingMap.end(); ++it)
        {
            std::unordered_map<std::string, common::Parameter> item = (*it);
            service::VoiceRecordingData* data = new service::VoiceRecordingData();
            data->name = std::string(item["name"]);
            data->time = std::string(item["time"]);
            data->duration = item["duration"];
            mRecordingData.push_back(data);     
        }

        auto deleteRecordingMap = repo[common::ParameterIndex::FMem_DeleteRecording].toList();
        for (auto it = deleteRecordingMap.begin(); it != deleteRecordingMap.end(); ++it)
        {
            std::unordered_map<std::string, common::Parameter> item = (*it);
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