#include "common/EventLoop.h"

int main()
{
    driver::DriverConfiguration driverConfig;
    driver::common::EventLoop::initialize(driverConfig);
    
    return driver::common::EventLoop::instance().exec();
}