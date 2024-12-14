#include "PhoneBookServiceImpl.h"
#include <utils/Logger.h>

namespace service {

PhoneBookServiceImpl::PhoneBookServiceImpl()
{
    mSIMDriver = driver::SIMDriver::getInstance();
    mDeploy = PhoneBookServiceDeploy::instance();
    mProvider = base::shm::PhoneBookProvider::instance();
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
            onPhoneContactListUpdated();
            onPhoneHistoryListUpdated();
            break;
        }
        }
    }
}

void PhoneBookServiceImpl::initialize()
{
    LOG_INFO("PhoneBookServiceImpl initialize");
    Connection::connect(mSIMDriver->onDriverReady, std::bind(&PhoneBookServiceImpl::onSIMDriverReady, this));
    Connection::connect(mSIMDriver->onPhoneContactListUpdated, std::bind(&PhoneBookServiceImpl::onPhoneContactListUpdated, this));
    Connection::connect(mSIMDriver->onPhoneHistoryListUpdated, std::bind(&PhoneBookServiceImpl::onPhoneHistoryListUpdated, this));
    
    mSIMDriver->connectDriver();
}

void PhoneBookServiceImpl::finialize()
{
    LOG_INFO("PhoneBookServiceImpl finialize");
}

void PhoneBookServiceImpl::registerClient(const std::string& clientName)
{
    if (mDeploy->registerClient(clientName))
    {
        mDeploy->responseServiceReady(clientName);
    }
}

void PhoneBookServiceImpl::onSIMDriverReady()
{
    // To do
}

void PhoneBookServiceImpl::onPhoneContactListUpdated()
{
    auto contactList = mSIMDriver->getContactList();
    auto result = mProvider->setPhoneContactList(contactList);
    if (result)
    {
        mDeploy->responsePhoneContactListUpdated();
    }
}

void PhoneBookServiceImpl::onPhoneHistoryListUpdated()
{
    auto historyList = mSIMDriver->getHistoryList();
    auto result = mProvider->setPhoneHistoryList(historyList);
    if (result)
    {
        mDeploy->responsePhoneHistoryListUpdated();
    }
}

}