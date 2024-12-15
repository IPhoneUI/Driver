#include "EasyMathServiceImpl.h"
#include <utils/Logger.h>

namespace service {

EasyMathServiceImpl::EasyMathServiceImpl()
    : BaseServiceImpl(EasyMathServiceDeploy::instance())
{
    mDeploy = EasyMathServiceDeploy::instance();
    mEasyMathServer = driver::EasyMathServer::getInstance();
}

void EasyMathServiceImpl::onMsqReceived()
{
    LOG_INFO("THAIVD --- EasyMathServiceImpl::onMsqReceived");
    std::vector<std::string> messages = mMqReceiver.receive("/EasyMathReq");
    if (!messages.empty()) {
        base::msq::Msq_EasyMathReq type = static_cast<base::msq::Msq_EasyMathReq>(mMqReceiver.get<int>(messages[0]));
        switch (type) {
        case base::msq::EasyMath_RegisterClient: {
            LOG_INFO("THAIVD --- EasyMathServiceImpl::onMsqReceived 111111111");
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            registerClient(clientName);
            break;
        }
        case base::msq::EasyMath_ReqStartGame: {
            mEasyMathServer->startGame();
            break;
        }
        default:
            break;
        }
    }

}

void EasyMathServiceImpl::initialize()
{
    LOG_INFO("EasyMathServiceImpl initialize");
    Connection::connect(mEasyMathServer->onStartGame, std::bind(&EasyMathServiceImpl::onStartGame, this, std::placeholders::_1));
}

void EasyMathServiceImpl::finialize()
{
    LOG_INFO("EasyMathServiceImpl finialize");
}

void EasyMathServiceImpl::registerClient(const std::string &clientName)
{
    if (mDeploy->registerClient(clientName)) {
        mDeploy->responseServiceReady(clientName);
    }
}

void EasyMathServiceImpl::onStartGame(const bool &isStart)
{
    mDeploy->responseStartGame(isStart);
}

}
