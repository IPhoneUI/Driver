#ifndef FLASHMEMORYDEPLOY_H
#define FLASHMEMORYDEPLOY_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseDeploy.h>
#include "FlashMemoryDef.h"

namespace driver {

class FlashMemoryDeploy : public common::BaseDeploy
{
public:
    static FlashMemoryDeploy* instance();

    void syncSysSetting(const AirplaneMode& airPlain);

private:
    explicit FlashMemoryDeploy();
};

}

#endif //FLASHMEMORYDEPLOY_H