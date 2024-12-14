#ifndef SPEAKERDRIVER_H
#define SPEAKERDRIVER_H

#include <string>
#include <thread>
#include <functional>
#include <common/BaseDriver.h>
#include <speaker/SpeakerProvider.h>

namespace driver {

class SpeakerDriver : public common::BaseDriver 
{
public:
    static SpeakerDriver* getInstance();
    static void initialize();
    void connectDriver() override;

    void readDataFromDatabase();
    
    bool getIsMuted() const
    {
        return mIsMuted;
    }

    int getVolume() const
    {
        return mVolume;
    }

    void requestChangeAudioMute(bool isMuted);

    Signal<bool> onMuteUpdated;
    Signal<int> onVolumeUpdated;
    
private:    
    explicit SpeakerDriver();

    bool mIsMuted {false};
    int mVolume {0};
};

}

#endif //SPEAKERDRIVER_H
