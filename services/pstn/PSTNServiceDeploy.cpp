#include "PSTNServiceDeploy.h"
#include <utils/Logger.h>

namespace service {

static PSTNServiceDeploy* gInstance = nullptr;

PSTNServiceDeploy::PSTNServiceDeploy()
{
}

PSTNServiceDeploy::~PSTNServiceDeploy()
{
}

PSTNServiceDeploy* PSTNServiceDeploy::instance()
{
    if (gInstance == nullptr)
    {
        gInstance = new PSTNServiceDeploy();
    }
    return gInstance;
}

void PSTNServiceDeploy::responseDriverReady(const std::string& clientName)
{
    std::lock_guard<std::mutex> lock(mMutex);
    mMqSender.startMsq(base::msq::Msq_PSTN_RespReady);
    mMqSender.sendMsq(clientName);
}

void PSTNServiceDeploy::responseCallStatusUpdated(service::CallStatus status, const std::string& number)
{
    mClientManager.deploy([this, status, number](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Msq_PSTN_RespCallStatusUpdated);
        mMqSender.addParam(static_cast<int>(status));
        mMqSender.addParam(number.c_str());
        mMqSender.sendMsq(mqName);
    });
}

void PSTNServiceDeploy::responseCallInfoUpdated(const std::string& number, const std::string& name, const std::string& avatar)
{
    mClientManager.deploy([this, number, name, avatar](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Msq_PSTN_RespCallInfoUpdated);
        mMqSender.addParam(number.c_str());
        mMqSender.addParam(name.c_str());
        mMqSender.addParam(avatar.c_str());
        mMqSender.sendMsq(mqName);
    });
}

void PSTNServiceDeploy::responseTimeUpdated(int time)
{
    mClientManager.deploy([this, time](std::string mqName) {
        std::lock_guard<std::mutex> lock(mMutex);
        mMqSender.startMsq(base::msq::Msq_PSTN_RespTimeUpdated);
        mMqSender.addParam(time);
        mMqSender.sendMsq(mqName);
    });
}

}
