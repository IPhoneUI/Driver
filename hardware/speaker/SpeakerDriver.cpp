#include "SpeakerDriver.h"
#include <utils/Logger.h>

namespace driver {

SpeakerDriver::SpeakerDriver()
{
}

void SpeakerDriver::onMsqReceived()
{
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