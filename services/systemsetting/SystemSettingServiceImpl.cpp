#include "SystemSettingServiceImpl.h"
#include <utils/Logger.h>

namespace service {

SystemSettingServiceImpl::SystemSettingServiceImpl()
    : common::BaseServiceImpl(SystemSettingServiceDeploy::instance())
{
    mDeploy = SystemSettingServiceDeploy::instance();
    mFMemDriver = driver::FlashMemoryDriver::getInstance();
}

void SystemSettingServiceImpl::onMsqReceived()
{
    std::vector<std::string> messages = mMqReceiver.receive("/sysSettingReq");
    if (!messages.empty()) 
    {
        base::msq::Msq_SysSett_Req type = static_cast<base::msq::Msq_SysSett_Req>(mMqReceiver.get<int>(messages[0]));
        switch (type) {
        case base::msq::Msq_SysSett_ReqRegisterClient: {
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            registerClient(clientName);
            break;
        }
        case base::msq::Msq_SysSett_ReqSync: {
            requestSync();
            break;
        }
        case base::msq::Msq_SysSett_ReqChangeAirPlaneMode: {
            bool airplane = mMqReceiver.get<bool>(messages[1]);
            mFMemDriver->changeAirPlaneMode(airplane);
            break;
        }
        }
    }
}

void SystemSettingServiceImpl::initialize()
{
    LOG_INFO("SystemSettingServiceImpl initialize");
    Connection::connect(mFMemDriver->onDriverReady, std::bind(&SystemSettingServiceImpl::onFMemDriverReady, this));
    Connection::connect(mFMemDriver->onAirPlaneModeUpdated, std::bind(&SystemSettingServiceImpl::onAirPlaneModeUpdated, this, std::placeholders::_1));
    
    mFMemDriver->connectDriver();
}

void SystemSettingServiceImpl::finialize()
{
    LOG_INFO("SystemSettingServiceImpl finialize");
}

void SystemSettingServiceImpl::requestSync()
{
    bool airPlane = mFMemDriver->getAirPlaneMode();
    mDeploy->responseChangeAirPlaneMode(airPlane);
}

void SystemSettingServiceImpl::onFMemDriverReady()
{
    // To do
}

void SystemSettingServiceImpl::onAirPlaneModeUpdated(bool airPlane)
{
    mDeploy->responseChangeAirPlaneMode(airPlane);
}

}