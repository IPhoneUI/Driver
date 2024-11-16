#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <iostream>
#include <csignal>
#include <cstdlib>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <chrono>

namespace driver::common {

class EventLoop {
public:
    static EventLoop& instance();

    void start();
    void stop();
    int join();
    static int exec();
    static void terminate(int signal);

private:
    explicit EventLoop();

    std::mutex mMutex;
    std::condition_variable mEventCV;
    std::thread mEventLoopThread;
    bool mRunning = false;
};

}


#endif