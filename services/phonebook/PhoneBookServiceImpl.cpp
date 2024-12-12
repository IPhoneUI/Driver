#include "PhoneBookServiceImpl.h"
#include <utils/Logger.h>

namespace service {

PhoneBookServiceImpl::PhoneBookServiceImpl()
{
    mSIMDriver = driver::SIMDriver::getInstance();
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
            mSIMDriver->requestSync(driver::SIMDriver::PhoneBookService);
            break;
        }
        }
    }
}

void PhoneBookServiceImpl::initialize()
{
    LOG_INFO("PhoneBookServiceImpl initialize");
    Connection::connect(mSIMDriver->onPhoneContactListUpdated, std::bind(&PhoneBookServiceImpl::onPhoneContactListUpdated, this));
    Connection::connect(mSIMDriver->onPhoneHistoryListUpdated, std::bind(&PhoneBookServiceImpl::onPhoneHistoryListUpdated, this));
}

void PhoneBookServiceImpl::finialize()
{
    LOG_INFO("PhoneBookServiceImpl finialize");
}

void PhoneBookServiceImpl::registerClient(const std::string& clientName)
{
    if (mDeploy->registerClient(clientName))
    {
        if (mIsDriverReady)
        {
            mDeploy->responseDriverReady(clientName);
        }
        else 
        {
            mClientWaitReady.push_back(clientName);
            mSIMDriver->connectDriver();
        }
    }
}

void PhoneBookServiceImpl::onDriverReady()
{
    mIsDriverReady = true;
    for (const auto& client : mClientWaitReady)
    {
        mDeploy->responseDriverReady(client);
    }
    mClientWaitReady.clear();
}

void PhoneBookServiceImpl::onPhoneContactListUpdated()
{
    auto contactList = mSIMDriver->getContactList();
}

void PhoneBookServiceImpl::onPhoneHistoryListUpdated()
{
    auto historyList = mSIMDriver->getHistoryList();
}

}