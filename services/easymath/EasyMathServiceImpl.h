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

namespace service {

class MonitorProgress {
public:
    MonitorProgress() {
        // mThreadTracking = std::thread(&execute);

    }
    MonitorProgress(const int interval) : mInterval(interval) {}

    void setInterval(const int interval) { mInterval = interval; }

    enum class State {
        Waiting,
        Incorrect,
        Correct,
        None
    };

    void setCommander(std::function<void(int)> func) {
        mCommander = func;
    }

    void setState(State state) { mState = state; }

    void execute(milliseconds deltal) {

        LOG_INFO("HEHEHEEH");
        if (mIsStart == true) {
            static milliseconds timeOut = milliseconds(0);

            timeOut += deltal;

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
        }
    }

    void start() {
        mIsStart = true;
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
    void onStartGame(const bool& isStart);
    void onExpressionChanged(const service::ExpressionInfo& info);
    void onAnswerResult(const bool& result);

    void submitCommand(int command);


private:
    EasyMathServiceDeploy* mDeploy;
    driver::EasyMathServer* mEasyMathServer;
    MonitorProgress* mMonitors {nullptr};
};

}

#endif //EASYMATHSERVICEIMPL_H
