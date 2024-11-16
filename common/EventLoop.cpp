#include "EventLoop.h"

namespace driver::common {

static EventLoop* gInstance = nullptr;

EventLoop& EventLoop::instance()
{
    if (gInstance == nullptr)
    {
        gInstance = new EventLoop();
    }
    return *gInstance;
}

EventLoop::EventLoop()
    : mEventLoopThread([this]() { start(); })
{
    std::signal(SIGINT, terminate);
}

void EventLoop::start()
{
    mRunning = true;
    while (mRunning) {
        std::unique_lock<std::mutex> lock(mMutex);
        
        mEventCV.wait(lock, [this] { return !mRunning; });
    }
}

void EventLoop::stop()
{
    std::lock_guard<std::mutex> lock(mMutex);
    mRunning = false;
    mEventCV.notify_all();
}

int EventLoop::join()
{
    if (mEventLoopThread.joinable())
    {
        mEventLoopThread.join();
        return 1;
    }
    return 0;
}

int EventLoop::exec()
{
    return gInstance->join();
}

void EventLoop::terminate(int signal)
{
    if (signal == SIGINT) {
        std::cout << "\nExiting program\n";
        gInstance->stop();
        std::exit(0);
    }
}

}