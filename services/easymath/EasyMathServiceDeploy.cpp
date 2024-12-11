#include "EasyMathServiceDeploy.h"
#include <utils/Logger.h>

namespace service {

static EasyMathServiceDeploy* gInstance = nullptr;

EasyMathServiceDeploy::EasyMathServiceDeploy()
{
}

EasyMathServiceDeploy::~EasyMathServiceDeploy()
{
}

EasyMathServiceDeploy* EasyMathServiceDeploy::instance()
{
    if (gInstance == nullptr)
    {
        gInstance = new EasyMathServiceDeploy();
    }
    return gInstance;
}

}
