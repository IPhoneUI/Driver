#ifndef PHONEBOOKSERVICEIMPL_H
#define PHONEBOOKSERVICEIMPL_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseServiceImpl.h>
#include <sim/SIMProvider.h>
#include "PhoneBookServiceDeploy.h"

namespace service {

class PhoneBookServiceImpl : public common::BaseServiceImpl 
{
public:
    explicit PhoneBookServiceImpl();

    void onMsqReceived() override;
    void initialize() override;
    void finialize() override;

    void registerClient(const std::string& clientName) override;


private:
    base::shm::SIMProvider* mSIMProvider;
    PhoneBookServiceDeploy* mDeploy;
};

}

#endif //PHONEBOOKSERVICEIMPL_H