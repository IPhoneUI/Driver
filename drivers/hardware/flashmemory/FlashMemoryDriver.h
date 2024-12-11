#ifndef FLASHMEMORYDRIVER_H
#define FLASHMEMORYDRIVER_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseDriver.h>
#include <flashmemory/FlashMemoryProvider.h>

namespace driver {

class FlashMemoryDriver : public common::BaseDriver 
{
public:
    static FlashMemoryDriver* getInstance();
    static void initialize();
    void connectDriver() override;

    // void registerClient(base::msq::Msq_Client clientId, const std::string& clientName);
    // void requestSync(base::msq::Msq_FlashMemoryReq type, const std::string& clientName);
    void requestChangeAirPlaneMode(bool airPlane);

private:
    explicit FlashMemoryDriver();

    base::shm::FlashMemoryProvider* mProvider;
};

}

#endif //FLASHMEMORYDRIVER_H