#ifndef FLASHMEMORYDEPLOY_H
#define FLASHMEMORYDEPLOY_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseDeploy.h>

namespace driver {

class FlashMemoryDeploy : public common::BaseDeploy
{
public:
    static FlashMemoryDeploy* instance();

    void requestChangeAirPlaneMode(bool airPlane);

private:
    explicit FlashMemoryDeploy();
};

}

#endif //FLASHMEMORYDEPLOY_H