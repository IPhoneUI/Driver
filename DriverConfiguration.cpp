#include "DriverConfiguration.h"

DriverConfiguration::DriverConfiguration()
{
    mThread = std::thread(&DriverConfiguration::execute, this);
    mThread.detach();
}

DriverConfiguration::~DriverConfiguration()
{
}

int DriverConfiguration::execute()
{
    while(true) {
        std::cout << "thread is running";
    }
}