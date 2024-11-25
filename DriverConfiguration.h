#ifndef DRIVERCONFIGURATION_H
#define DRIVERCONFIGURATION_H

#include <thread>
#include <functional>
#include <sim/SIMDriver.h>
#include <speaker/SpeakerDriver.h>
#include <easymath/EasyMathServer.h>
#include <flashmemory/FlashMemoryDriver.h>
#include <wifi/WifiDriver.h>

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