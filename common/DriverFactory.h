#ifndef DRIVERFACTORY_H
#define DRIVERFACTORY_H

#include <BaseDriver.h>
#include <vector>

namespace driver::common {

class DriverFactory {
public:
    ~DriverFactory();
    static DriverFactory& instance();

    template <typename T>
    T *addDriver()
    {
        T* ins = new T();
        mDrivers.push_back(ins);

        return ins;
    }

    void initialize();
    void finialize();

private:
    DriverFactory();

    std::vector<BaseDriver*> mDrivers;
};

}

#endif // DRIVERFACTORY_H
