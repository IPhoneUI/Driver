#include "FlashMemoryDriver.h"
#include <utils/Logger.h>

namespace driver {

FlashMemoryDriver::FlashMemoryDriver() : 
    mThread(std::bind(&FlashMemoryDriver::execute, this))
{
    mFlMemoryDeploy = FlashMemoryDeploy::instance();
    mThread.detach();
}

void FlashMemoryDriver::execute()
{
    while(true)
    {
        if (mMqReceiver.receive("/flashmemoryReq")) 
        {
            std::vector<std::string> messages = mMqReceiver.getMessage();
            LOG_INFO("id: %d", mMqReceiver.get<int>(messages[0]));

            int type = mMqReceiver.get<int>(messages[0]);
            switch (type) {
            case service::SysSett_RequestSync:
                mFlMemoryDeploy->syncSysSetting(mAirplaneMode);
                break;
            }
        }
    }
}

void FlashMemoryDriver::initialize()
{
    LOG_INFO("FlashMemoryDriver initialize");
}

void FlashMemoryDriver::finialize()
{
    LOG_INFO("FlashMemoryDriver finialize");
}

}