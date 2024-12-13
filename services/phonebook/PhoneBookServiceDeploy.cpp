#include "PhoneBookServiceDeploy.h"
#include <utils/Logger.h>

namespace service {

static PhoneBookServiceDeploy* gInstance = nullptr;

PhoneBookServiceDeploy::PhoneBookServiceDeploy()
{
}

PhoneBookServiceDeploy::~PhoneBookServiceDeploy()
{
}

PhoneBookServiceDeploy* PhoneBookServiceDeploy::instance()
{
    if (gInstance == nullptr)
    {
        gInstance = new PhoneBookServiceDeploy();
    }
    return gInstance;
}

void PhoneBookServiceDeploy::responseDriverReady(const std::string& clientName)
{
    std::lock_guard<std::mutex> lock(mMutex);
    mMqSender.startMsq(base::msq::Msq_PhoneBook_RespReady);
    mMqSender.sendMsq(clientName);
}

void PhoneBookServiceDeploy::responsePhoneContactListUpdated()
{
    mClientManager.deploy([this](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Msq_PhoneBook_RespContactListUpdated);
        mMqSender.sendMsq(mqName);
    });
}

void PhoneBookServiceDeploy::responsePhoneHistoryListUpdated()
{
    mClientManager.deploy([this](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Msq_PhoneBook_RespHistoryListUpdated);
        mMqSender.sendMsq(mqName);
    });
}

}
