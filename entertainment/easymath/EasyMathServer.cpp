#include "EasyMathServer.h"
#include <utils/Logger.h>

namespace driver {

EasyMathServer::EasyMathServer() : mThread(std::bind(&EasyMathServer::execute, this))
{
    mThread.detach();
}

void EasyMathServer::execute()
{
    while(true)
    {
    }
}

void EasyMathServer::initialize()
{
    LOG_INFO("EasyMathServer initialize");
}

void EasyMathServer::finialize()
{
    LOG_INFO("EasyMathServer finialize");
}

}