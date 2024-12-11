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
};

}

#endif