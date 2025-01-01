#include "MonitorProgress.h"

namespace driver {

MonitorProgress::MonitorProgress()
{
}

MonitorProgress::MonitorProgress(const int interval) : mInterval(interval)
{
}

void MonitorProgress::setInterval(int interval)
{
    if (mInterval == interval)
        return;
    
    mInterval = interval;
}

void MonitorProgress::setCommander(std::function<void(int)> func)
{
    if (func != nullptr) 
    {
        mCommander = func;
    }
}

void MonitorProgress::setState(State state)
{
    if (mState != state) 
    {
        mState = state;
    }
}

void MonitorProgress::execute(milliseconds delta)
{
    if (mIsStart) 
    {
        mTimeout += delta;
        switch (mState) {
        case State::Incorrect: {
            mCommander(static_cast<int>(GameEvent::GameOver));
            setState(State::None);
            mIsStart = false;
            mTimeout = milliseconds(0);
            break;
        }
        case State::Correct: {
            mCommander(static_cast<int>(GameEvent::NextLevel));
            setState(State::None);
            mTimeout = milliseconds(0);
            break;
        }
        }

        if (mTimeout.count() > 8000) 
        {
            std::unique_lock<std::mutex> lock(mMutex);
            mCommander(static_cast<int>(GameEvent::GameOver));
            setState(State::None);
            mIsStart = false;
            mTimeout = milliseconds(0);
        }
    }
}

void MonitorProgress::start()
{
    if (mIsStart == true) 
        return;
    
    mIsStart = true;
    mTimeout = milliseconds(0);
}

void MonitorProgress::stop() 
{
    mIsStart = false;
    mTimeout = milliseconds(0);
}

}