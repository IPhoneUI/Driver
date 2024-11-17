#include "SIMDriver.h"

namespace driver {

SIMDriver::SIMDriver() : mThread(std::bind(&SIMDriver::execute, this))
{
    mThread.detach();
}

void SIMDriver::execute()
{
    while(true)
    {
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