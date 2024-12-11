#ifndef EASYMATHSERVICEIMPL_H
#define EASYMATHSERVICEIMPL_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseServiceImpl.h>

namespace service {

class EasyMathServiceImpl : public common::BaseServiceImpl 
{
public:
    explicit EasyMathServiceImpl();

    void onMsqReceived() override;
    void initialize() override;
    void finialize() override;

    void registerClient(const std::string& clientName) {};

private:
};

}

#endif //EASYMATHSERVICEIMPL_H