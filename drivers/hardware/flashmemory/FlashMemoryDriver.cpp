#include "FlashMemoryDriver.h"
#include <utils/Logger.h>

namespace driver {

static FlashMemoryDriver* gInstance = 0;

FlashMemoryDriver::FlashMemoryDriver()
{
    mRepo.setName("flashmemory");
    mRepo.addParam("recording", common::ParameterIndex::FMem_Recording);
    mRepo.addParam("delete_recording", common::ParameterIndex::FMem_DeleteRecording);
    mRepo.addParam("airplane_mode", common::ParameterIndex::FMem_AirPlaneMode);

    Connection::connect(mRepo.onRepoStateChanged, std::bind(&FlashMemoryDriver::onRepoStateChanged, this, std::placeholders::_1));

    mRepo.pull();

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

void FlashMemoryDriver::writeBuffer()
{
    mRepo[common::ParameterIndex::FMem_AirPlaneMode] = mAirPlaneMode;
    utils::VariantList recordingList;
    for (const auto& recordingItem : mRecordingData)
    {
        std::unordered_map<std::string, utils::Variant> item;
        service::VoiceRecordingData* data = recordingItem;
        item["name"] = data->name;
        item["time"] = data->time;
        item["duration"] = data->duration;

        recordingList.push(item);
    }

    utils::VariantList deleteRecordingList; 
    for (const auto& recordingItem : mDeleteRecordingData)
    {
        std::unordered_map<std::string, utils::Variant> item;
        service::VoiceRecordingData* data = recordingItem;
        item["name"] = data->name;
        item["time"] = data->time;
        item["duration"] = data->duration;

        deleteRecordingList.push(item);
    }

    mRepo[common::ParameterIndex::FMem_Recording] = recordingList;
    mRepo[common::ParameterIndex::FMem_DeleteRecording] = deleteRecordingList;
}

void FlashMemoryDriver::onRepoStateChanged(common::Repository::State state)
{
    if (state == common::Repository::PullCompleted)
    {
        utils::VariantList recordings = mRepo[common::ParameterIndex::FMem_Recording];
        for (const auto &recording : recordings) 
        {
            std::unordered_map<std::string, utils::Variant> item = recording;
            service::VoiceRecordingData* data = new service::VoiceRecordingData();
            data->name = item["name"];
            data->time = item["time"];
            data->duration = item["duration"];
            mRecordingData.push_back(data);   
        }

        utils::VariantList deleteRecordings = mRepo[common::ParameterIndex::FMem_DeleteRecording];
        for (const auto &recording : deleteRecordings) 
        {
            std::unordered_map<std::string, utils::Variant> item = recording;
            service::VoiceRecordingData* data = new service::VoiceRecordingData();
            data->name = item["name"];
            data->time = item["time"];
            data->duration = item["duration"];
            mDeleteRecordingData.push_back(data);       
        }
        
        mAirPlaneMode = mRepo[common::ParameterIndex::FMem_AirPlaneMode];
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