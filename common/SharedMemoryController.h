#ifndef SHAREDMEMORYCONTROLLER_H
#define SHAREDMEMORYCONTROLLER_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <string.h>
#include <thread>
#include <functional>

namespace driver::common {

struct Person {
    std::string name;
    int age;
    std::string phone;
};

class SharedMemoryController
{
public:
    explicit SharedMemoryController();

    void execute();

private:
    std::thread mThread;
};

}

#endif //SHAREDMEMORYCONTROLLER_H