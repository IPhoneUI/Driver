#ifndef DRIVERCONFIGURATION_H
#define DRIVERCONFIGURATION_H

#include <thread>
#include <functional>

namespace driver {

class DriverConfiguration {
public:
    explicit DriverConfiguration();
    ~DriverConfiguration();

    void start();
    void stop();
    void initDriver();
    void addService();
};

}

#endif