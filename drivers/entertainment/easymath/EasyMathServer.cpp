#include "EasyMathServer.h"
#include <utils/Logger.h>

namespace driver {

static EasyMathServer* gInstance = 0;

EasyMathServer::EasyMathServer()
    : mMonitors(new MonitorProgress(8))
{
    mMonitors->setCommander(std::bind(&EasyMathServer::submitCommand, this, std::placeholders::_1));
    common::DriverExecution::instance().addDriver("EasyMathServer", this);
}

EasyMathServer* EasyMathServer::getInstance()
{
    if (gInstance == nullptr)
    {
        throw std::runtime_error("The EasyMathServer has not been initialized yet");
    }

    return gInstance;
}

void EasyMathServer::initialize()
{
    if (gInstance == nullptr)
    {
        gInstance = new EasyMathServer();
    }
}

void EasyMathServer::connectDriver()
{
    mIsReady = true;
    onDriverReady.emit();
}

void EasyMathServer::writeBuffer()
{
}

void EasyMathServer::execute(milliseconds delta)
{
    mMonitors->execute(delta);
}

void EasyMathServer::setRangeNumber(const int &range)
{
    if (mRangeNumber != range) {
        mRangeNumber = range;
    }
    onRangeNumberUpdated.emit();
}

int EasyMathServer::getRangeNumber() const
{
    return mRangeNumber;
}

void EasyMathServer::setHighestScore(const int &score)
{
    if (mHighestCore != score) {
        mHighestCore = score;
    }
    onHighestScoreUpdated.emit();
}

int EasyMathServer::getHighestScore() const
{
    return mHighestCore;
}

void EasyMathServer::startGame()
{
    if (mIsGameRunning == true)
        return;

    onTimeIntervalChanged.emit(8000);
    mIsGameRunning = true;
    mLevel = 1;
    onStartGame.emit();
    generateExpression();
}

void EasyMathServer::generateExpression()
{
    std::vector<service::ExpressionType> operators = {service::ExpressionType::Addition, service::ExpressionType::Subtraction, service::ExpressionType::Multiplication};
    size_t m_nRandomOperator = floor(((float) rand() / (RAND_MAX)) * operators.size());
    service::ExpressionType expr = operators[m_nRandomOperator];

    int firstNumber = getRandomNumber(1 * mLevel, 5 * mLevel);
    int secondNumber = getRandomNumber(1 * mLevel, 5 * mLevel);
    int result = 0;

    switch (expr) {
    case service::ExpressionType::Addition:
        result = firstNumber + secondNumber;
        break;
    case service::ExpressionType::Subtraction:
        result = firstNumber - secondNumber;
        break;
    case service::ExpressionType::Multiplication:
        result = firstNumber * secondNumber;
        break;
    default:
        break;
    }

    mResult = result;

    mExprInfo.firstNumber = firstNumber;
    mExprInfo.secondNumber = secondNumber;
    mExprInfo.dummyResult = getResult();
    mExprInfo.exprType = expr;

    onExpressionChanged.emit(mExprInfo);
    mMonitors->start();
}

int EasyMathServer::getRandomNumber(const int &min, const int &max)
{
    return floor(((float) rand() / (RAND_MAX)) * (max - min) + min);
}

void EasyMathServer::requestCheckingResult(const bool &result)
{
    if (mRandomResult == result) 
    {
        mMonitors->setState(MonitorProgress::State::Correct);
    } 
    else 
    {
        mMonitors->setState(MonitorProgress::State::Incorrect);
    }
}

int EasyMathServer::getResult()
{
    mRandomResult = ((float) rand() / (RAND_MAX)) >= 0.5;
    if (mRandomResult == false) {
        mResult = getRandomNumber(mResult - 10, mResult + 10);
    }
    return mResult;
}

void EasyMathServer::resetGame()
{
    mIsGameRunning = false;
    mResult = 0;
    mRandomResult = 0;
    mLevel = 0;
    mRangeNum = 10;
    mScore = 0;
}

void EasyMathServer::nextLevel()
{
    mLevel++;
    mRangeNum += 5;
    mScore += 3;
    generateExpression();
}

void EasyMathServer::submitCommand(int command)
{
    MonitorProgress::GameEvent event = static_cast<MonitorProgress::GameEvent>(command);
    switch (event)
    {
    case MonitorProgress::GameEvent::GameOver:
        onGameOver.emit(mScore);
        mMonitors->stop();
        resetGame();
        break;
    case MonitorProgress::GameEvent::NextLevel:
        nextLevel();
        break;
    default:
        break;
    }
}

}
