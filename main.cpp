#include <iostream>
#include "common/EventLoop.h"

int main()
{
    std::cout << "Driver is initialized" << std::endl;
    driver::DriverConfiguration driverConfig;
    driver::common::EventLoop::initialize(driverConfig);
    
    return driver::common::EventLoop::instance().exec();
}