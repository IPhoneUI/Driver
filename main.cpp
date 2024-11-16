#include <iostream>
#include <csignal>
#include <cstdlib>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <chrono>
#include "common/EventLoop.h"

int main()
{
    std::cout << "Driver is initialized" << std::endl;
    // driver::common::EventLoop::instance();
    
    return driver::common::EventLoop::instance().exec();
}