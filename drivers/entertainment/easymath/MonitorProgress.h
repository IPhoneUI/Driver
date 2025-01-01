#ifndef MONITORPROGRESS_H
#define MONITORPROGRESS_H

#include <string>
#include <thread>
#include <functional>
#include <easymath/EasyMathDef.h>
#include "BaseDriver.h"
#include <utility>
#include <chrono>
#include <cmath>

namespace driver {

class MonitorProgress {
public:
    MonitorProgress();
    MonitorProgress(const int interval);

    enum class GameEvent {
        Connect = 0,
        Disconnect,
        StartGame,
        GenerateExpresion,
        SubmitResult,
        GameOver,
        StartProgressBar,
        NextLevel,
        None,
    };

    enum State {
        Waiting,
        Incorrect,
        Correct,
        None
    };

    int interval() const
    {
        return mInterval;
    }


    void setInterval(int interval);
    void setCommander(std::function<void(int)> func);
    void setState(State state);
    void execute(milliseconds delta);
    void start();
    void stop();

private:
    std::mutex mMutex;
    int mInterval {0};
    State mState {State::None};
    std::function<void(int)> mCommander;
    bool mIsStart {false};
    milliseconds mTimeout;
};

}

#endif