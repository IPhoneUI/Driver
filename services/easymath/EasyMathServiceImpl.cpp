#include "EasyMathServiceImpl.h"
#include <utils/Logger.h>

namespace service {

EasyMathServiceImpl::EasyMathServiceImpl()
    : BaseServiceImpl(EasyMathServiceDeploy::instance())
{
    mDeploy = EasyMathServiceDeploy::instance();
    mEasyMathServer = driver::EasyMathServer::getInstance();
}

void EasyMathServiceImpl::registerClient(const std::string &clientName)
{
    if (mDeploy->registerClient(clientName)) {
        mDeploy->responseServiceReady(clientName);
    }
}

void EasyMathServiceImpl::onMsqReceived()
{
    std::vector<std::string> messages = mMqReceiver.receive("/EasyMathReq");
    if (!messages.empty()) {
        base::msq::Msq_EasyMathReq type = static_cast<base::msq::Msq_EasyMathReq>(mMqReceiver.get<int>(messages[0]));
        switch (type) {
        case base::msq::Msq_EasyMath_RegisterClient: {
            std::string clientName = mMqReceiver.get<std::string>(messages[1]);
            registerClient(clientName);
            break;
        }
        case base::msq::Msq_EasyMath_ReqStartGame: {
            mEasyMathServer->startGame();
            break;
        }
        case base::msq::Msq_EasyMath_ReqCheckResult: {
            bool result = mMqReceiver.get<bool>(messages[1]);
            mEasyMathServer->requestCheckingResult(result);
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
    Connection::connect(mEasyMathServer->onStartGame, std::bind(&EasyMathServiceImpl::onStartGame, this));
    Connection::connect(mEasyMathServer->onExpressionChanged, std::bind(&EasyMathServiceImpl::onExpressionChanged, this, std::placeholders::_1));
    Connection::connect(mEasyMathServer->onHighestScoreUpdated, std::bind(&EasyMathServiceImpl::onHighestScoreChanged, this));
    Connection::connect(mEasyMathServer->onRangeNumberUpdated, std::bind(&EasyMathServiceImpl::onRangeNumberChanged, this));
    Connection::connect(mEasyMathServer->onGameOver, std::bind(&EasyMathServiceImpl::onGameOver, this, std::placeholders::_1));
    Connection::connect(mEasyMathServer->onTimeIntervalChanged, std::bind(&EasyMathServiceImpl::onTimeIntervalChanged, this, std::placeholders::_1));
}

void EasyMathServiceImpl::finialize()
{
    LOG_INFO("EasyMathServiceImpl finialize");
}

void EasyMathServiceImpl::onStartGame()
{
    mDeploy->responseStartGame();
}

void EasyMathServiceImpl::onExpressionChanged(const service::ExpressionInfo& info)
{
    mDeploy->responseExpressionChanged(info);
}

void EasyMathServiceImpl::onHighestScoreChanged()
{
    // mScore = mEasyMathServer->getHighestScore();
}

void EasyMathServiceImpl::onRangeNumberChanged()
{
    // mRangeNum = mEasyMathServer->getRangeNumber();
}

void EasyMathServiceImpl::onTimeIntervalChanged(size_t time)
{
    mDeploy->responseTimeIntervalUpdated(time);
}

void EasyMathServiceImpl::onGameOver(int score)
{
    mDeploy->responseGameOver(score);
}

}
