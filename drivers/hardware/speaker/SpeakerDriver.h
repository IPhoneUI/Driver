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
    static SpeakerDriver* getInstance();
    static void initialize();
    
    void connectDriver() override;
    void writeBuffer() override;
    
    bool getIsMuted() const
    {
        return mIsMuted;
    }

    int getVolume() const
    {
        return mVolume;
    }

    void changeAudioMute(bool isMuted);

    Signal<bool> onMuteUpdated;
    Signal<int> onVolumeUpdated;
    
private: 
    void onRepoStateChanged(common::Repository::State state);

private:    
    explicit SpeakerDriver();
    common::Repository mRepo;

    bool mIsMuted {false};
    int mVolume {0};
};

}

#endif //SPEAKERDRIVER_H
