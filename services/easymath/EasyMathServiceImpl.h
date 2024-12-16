#ifndef EASYMATHSERVICEIMPL_H
#define EASYMATHSERVICEIMPL_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseServiceImpl.h>
#include "EasyMathServiceDeploy.h"
#include <easymath/EasyMathServer.h>
#include <utility>
#include <chrono>
#include <cmath>

namespace service {

using time_clock = std::chrono::system_clock;

class MonitorProgress {
public:
    MonitorProgress()
    {
    }

    MonitorProgress(const int interval) : mInterval(interval)
    {
    }

    void setInterval(const int interval)
    {
        mInterval = interval;
    }

    int getInterval() const
    {
        return mInterval;
    }

    enum State {
        Waiting,
        Incorrect,
        Correct,
        None
    };

    void setCommander(std::function<void(int)> func)
    {
        if (func != nullptr) {
            mCommander = func;
        }
    }

    void setState(State state)
    {
        if (mState != state) {
            mState = state;
        }
    }

    void trackingWorker()
    {
        static time_clock::time_point lastUpdate = time_clock::now();

        static milliseconds timeOut = milliseconds(0);

        while (mIsStart) {

            milliseconds delta = std::chrono::duration_cast<milliseconds>(time_clock::now() - lastUpdate);

            lastUpdate = time_clock::now();

            timeOut += delta;

            if (mState == State::Incorrect) {
                mCommander(static_cast<int>(GameEvent::GameOver));
                setState(State::None);
                mIsStart = false;
            } else if (mState == State::Correct) {
                mCommander(static_cast<int>(GameEvent::NextLevel));
                setState(State::None);
                timeOut = milliseconds(0);
            }

            if (timeOut.count() > 8000) {
                std::unique_lock<std::mutex> lock(mMutex);
                mCommander(static_cast<int>(GameEvent::GameOver));
                setState(State::None);
                mIsStart = false;
            }

            usleep(10000);
        }
    }

    void start()
    {
        if (mIsStart == true) {
            return;
        }
        mIsStart = true;
        mThreadTracking = std::thread(&MonitorProgress::trackingWorker, this);
        mThreadTracking.detach();
    }

    void stop() {
        mIsStart = false;
    }

    std::mutex mMutex;
    std::thread mThreadTracking;
    int mInterval {0};
    State mState {State::None};
    std::function<void(int)> mCommander;
    bool mIsStart {false};
};

class EasyMathServiceImpl : public common::BaseServiceImpl 
{
public:
    explicit EasyMathServiceImpl();

    void onMsqReceived() override;
    void initialize() override;
    void finialize() override;
    void registerClient(const std::string& clientName);

    void startGame();
    void generateExpression();
    int getRandomNumber(const int& min, const int& max);
    void requestCheckingResult(const bool& result);
    int getResult();
    void resetGame();
    void nextLevel();

    void onHighestScoreChanged();
    void onRangeNumberChanged();
    void submitCommand(int command);

private:
    EasyMathServiceDeploy* mDeploy;
    driver::EasyMathServer* mEasyMathServer;
    MonitorProgress* mMonitors {nullptr};

    service::ExpressionInfo mExprInfo;

    bool mIsGameRunning {false};
    int mResult {0};
    bool mRandomResult {false};
    int mFirstArgument {0};
    int mSecondArgument {0};
    int mResultDummy {0};
    int mLevel {0};
    int mRangeNum {10};
    std::mutex mMutex;
    int mScore {0};
};

}

#endif //EASYMATHSERVICEIMPL_H
