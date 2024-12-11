#include "PhoneBookServiceImpl.h"
#include <utils/Logger.h>

namespace service {

PhoneBookServiceImpl::PhoneBookServiceImpl()
{
    mSIMProvider = base::shm::SIMProvider::instance();
    mDeploy = PhoneBookServiceDeploy::instance();
}

void PhoneBookServiceImpl::onMsqReceived()
{
    std::vector<std::string> messages = mMqReceiver.receive("/phoneBookReq");
    if (!messages.empty()) 
    {
        base::msq::Msq_PhoneBook_Req type = static_cast<base::msq::Msq_PhoneBook_Req>(mMqReceiver.get<int>(messages[0]));
        switch (type) {
        case base::msq::Msq_PhoneBook_ReqRegisterClient: {
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            registerClient(clientName);
            break;
        }
        case base::msq::Msq_PhoneBook_ReqSync:{
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            mDeploy->responseSync(clientName);
            break;
        }
        }
    }
}

void PhoneBookServiceImpl::initialize()
{
    LOG_INFO("PhoneBookServiceImpl initialize");
    mSIMProvider->initialize();
}

void PhoneBookServiceImpl::finialize()
{
    LOG_INFO("PhoneBookServiceImpl finialize");
    mSIMProvider->closeShmem();
}

void PhoneBookServiceImpl::registerClient(const std::string& clientName)
{
    if (mDeploy->registerClient(clientName))
    {
        mDeploy->responseDriverReady(clientName);
    }
}

}