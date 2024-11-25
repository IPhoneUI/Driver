#include "SpeakerDriver.h"
#include <utils/Logger.h>

namespace driver {

SpeakerDriver::SpeakerDriver() : mThread(std::bind(&SpeakerDriver::execute, this))
{
    mThread.detach();
}

void SpeakerDriver::execute()
{
    while(true)
    {
    }
}

void SpeakerDriver::initialize()
{
    LOG_INFO("SpeakerDriver initialize");
}

void SpeakerDriver::finialize()
{
    LOG_INFO("SpeakerDriver finialize");
}

}