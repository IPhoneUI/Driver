#include "SIMDriver.h"
#include <utils/Logger.h>

namespace driver {

SIMDriver::SIMDriver() : mThread(std::bind(&SIMDriver::execute, this))
{
    mMqReceiver.registerMsq("/example_queue");
    mThread.detach();
}

void SIMDriver::execute()
{
    while(true)
    {
        std::vector<std::string> messages = mMqReceiver.receive();
        if (!messages.empty()) 
        {
            LOG_INFO("id: %d", mMqReceiver.get<int>(messages[0]));
            LOG_INFO("id: %d", mMqReceiver.get<int>(messages[1]));
            LOG_INFO("name: %s", mMqReceiver.get<std::string>(messages[2]).c_str());
            LOG_INFO("double: %f", mMqReceiver.get<double>(messages[3]));
            LOG_INFO("bool: %d", mMqReceiver.get<bool>(messages[4]));
        }
    }
}

void SIMDriver::initialize()
{
    std::cout << "SIMDriver initialize" << std::endl;
}

void SIMDriver::finialize()
{
    std::cout << "SIMDriver finialize" << std::endl;
}

}