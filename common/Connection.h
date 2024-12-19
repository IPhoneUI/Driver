#ifndef DRIVER_CONNECT_H
#define DRIVER_CONNECT_H

#include <functional>
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <mutex>

class Connection;
template<typename... Args>
class Signal {
    friend class Connection;
public:
    using Slot = std::function<void(Args...)>;

    void emit(Args... args) {
        std::lock_guard<std::mutex> lock(mMutex);
        for (auto& slot : mSlots) 
        {
            if (slot) 
            {
                slot(args...);
            }
        }
    }

protected:
    void connect(Slot slot) 
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mSlots.push_back(slot);
    }

private:
    std::mutex mMutex;
    std::vector<Slot> mSlots;
};

class Connection {
public:
    template<typename... Args>
    static void connect(Signal<Args...>& signal, Signal<Args...>::Slot slot) 
    {
        signal.connect(slot);
    }
};

#endif