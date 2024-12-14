#ifndef PHONEBOOKSERVICEDEPLOY_H
#define PHONEBOOKSERVICEDEPLOY_H

#include <string>
#include <thread>
#include <list>
#include <functional>
#include <mutex>
#include <shared_mutex>
#include <common/BaseDeploy.h>
#include <audio/AudioServiceDef.h>
#include <speaker/SpeakerProvider.h>

namespace service {

class PhoneBookServiceDeploy : public common::BaseDeploy
{
public:
    static PhoneBookServiceDeploy* instance();

    void responseServiceReady(const std::string& clientName);
    void responseSync(const std::string& clientName);
    void responsePhoneContactListUpdated();
    void responsePhoneHistoryListUpdated();

private:
    explicit PhoneBookServiceDeploy();
    ~PhoneBookServiceDeploy();

    std::mutex mMutex;
};

}

#endif //PHONEBOOKSERVICEDEPLOY_H
