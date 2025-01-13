#include "WeatherServiceImpl.h"
#include <msq/MsqDef.h>

namespace service {

WeatherServiceImpl::WeatherServiceImpl()
    : BaseServiceImpl(WeatherServiceDeploy::instance())
    , mDeploy(WeatherServiceDeploy::instance())
    , mDriver(driver::WeatherDriver::instance())
{

}

WeatherServiceImpl::~WeatherServiceImpl()
{

}

void WeatherServiceImpl::onMsqReceived()
{
    std::vector<std::string> messages = mMqReceiver.receive("/WeatherReq");
    if (!messages.empty()) {
        base::msq::Msq_WeatherReq req = static_cast<base::msq::Msq_WeatherReq>(mMqReceiver.get<int>(messages[0]));
        switch (req)
        {
        case base::msq::Msq_WeatherReq::Msq_Weather_RegisterClient: {
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            registerClient(clientName);
            break;
        }
        default:
            break;
        }
    }
}

void WeatherServiceImpl::initialize()
{

}

void WeatherServiceImpl::finialize()
{

}

void WeatherServiceImpl::registerClient(const std::string &clientName)
{
    if (mDeploy->registerClient(clientName)) {
        mDeploy->responseServiceReady(clientName);
    }
}


}
