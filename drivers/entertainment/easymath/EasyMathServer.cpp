#include "EasyMathServer.h"
#include <utils/Logger.h>

namespace driver {

static EasyMathServer* gInstance = 0;

EasyMathServer::EasyMathServer()
{
}

EasyMathServer* EasyMathServer::getInstance()
{
    if (gInstance == nullptr)
    {
        throw std::runtime_error("The EasyMathServer has not been initialized yet");
        gInstance = new EasyMathServer();
    }

    return gInstance;
}

void EasyMathServer::initialize()
{
    LOG_INFO("EasyMathServer initialize");
    if (gInstance == nullptr)
    {
        gInstance = new EasyMathServer();
    }
}

void EasyMathServer::connectDriver()
{
    mIsReady = true;
    onDriverReady.emit();
}

}