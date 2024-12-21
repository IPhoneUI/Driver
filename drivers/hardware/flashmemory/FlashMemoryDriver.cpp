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
        throw std::runtime_error("The FlashMemoryDriver has not been initialized yet");
    }

    return gInstance;
}

void FlashMemoryDriver::initialize()
{
    if (gInstance == nullptr)
    {
        gInstance = new FlashMemoryDriver();
    }
}

void FlashMemoryDriver::connectDriver()
{
    mIsReady = true;
    onDriverReady.emit();
}

void FlashMemoryDriver::onRepoStateChanged(common::Repository::State state)
{
    if (state == common::Repository::PullCompleted)
    {
        common::DataRepoManager& dataRepo = common::DataRepoManager::instance();
        common::Repository& repo = dataRepo.repository("flashmemory");

        utils::VariantList recordings = repo[common::ParameterIndex::FMem_Recording];
        for (const auto &recording : recordings) 
        {
            std::unordered_map<std::string, utils::Variant> item = recording;
            service::VoiceRecordingData* data = new service::VoiceRecordingData();
            data->name = item["name"];
            data->time = item["time"];
            data->duration = item["duration"];
            mRecordingData.push_back(data);   
        }

        utils::VariantList deleteRecordings = repo[common::ParameterIndex::FMem_DeleteRecording];
        for (const auto &recording : deleteRecordings) 
        {
            std::unordered_map<std::string, utils::Variant> item = recording;
            service::VoiceRecordingData* data = new service::VoiceRecordingData();
            data->name = item["name"];
            data->time = item["time"];
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