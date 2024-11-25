#ifndef SPEAKERDRIVER_H
#define SPEAKERDRIVER_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseDriver.h>

namespace driver {

class SpeakerDriver : public common::BaseDriver 
{
public:
    explicit SpeakerDriver();

    void execute();
    void initialize() override;
    void finialize() override;

    bool isMute() const
    {
        return mIsMute;
    }

    size_t volume() const
    {
        return mVolume;
    }

private:
    std::thread mThread;
    
    bool mIsMute {false};
    size_t mVolume {16};
};

}

#endif //SPEAKERDRIVER_H