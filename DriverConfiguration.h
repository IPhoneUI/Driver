#ifndef DRIVERCONFIGURATION_H
#define DRIVERCONFIGURATION_H

#include <thread>
#include <functional>
#include <iostream>
#include "SIMDriver.h"

namespace driver {

class DriverConfiguration {
public:
    explicit DriverConfiguration();
    ~DriverConfiguration();

    void start();
    void stop();

private:
    SIMDriver* mSIMDriver;
};

}

#endif