#include "EasyMathServer.h"
#include <utils/Logger.h>

namespace driver {

static EasyMathServer* gInstance = 0;

EasyMathServer::EasyMathServer()
{
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

void EasyMathServer::writeData()
{

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

}
