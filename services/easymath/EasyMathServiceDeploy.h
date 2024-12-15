#ifndef EASYMATHSERVICEDEPLOY_H
#define EASYMATHSERVICEDEPLOY_H

#include <string>
#include <thread>
#include <list>
#include <functional>
#include <mutex>
#include <shared_mutex>
#include <common/BaseDeploy.h>
#include <audio/AudioServiceDef.h>

namespace service {

class EasyMathServiceDeploy : public common::BaseDeploy
{
public:
    static EasyMathServiceDeploy* instance();

    void responseServiceReady(const std::string& clientName);
    void responseStartGame(const bool& status);

private:
    explicit EasyMathServiceDeploy();
    ~EasyMathServiceDeploy();

    std::mutex mMutex;
};

}

#endif //EASYMATHSERVICEDEPLOY_H
