#ifndef DRIVERCONFIGURATION_H
#define DRIVERCONFIGURATION_H

#include <thread>
#include <functional>
#include <iostream>

class DriverConfiguration {
public:
    explicit DriverConfiguration();
    ~DriverConfiguration();

    int execute();

private:
    std::thread mThread;
};

#endif