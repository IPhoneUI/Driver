#ifndef STORAGE_DATAREPOMANAGER_H
#define STORAGE_DATAREPOMANAGER_H

#include <list>
#include <string>
#include <thread>
#include <vector>
#include <queue>
#include <list>
#include <variant>
#include <mutex>
#include <functional>
#include <unordered_map>
#include <mutex>
#include "Repository.h"
#include <utils/filesystem.h>

namespace driver::storages {
    
class DataRepoManager {
public:
    enum State {
        StandbyState,
        WaitToSyncDataState,
        ReadyState,
        PushingState,
    };

    static DataRepoManager& instance();

    bool isReady() const
    {
        return mState == ReadyState;
    }

    void setState(State state);


    void pull();
    void push();

    Repository* addRepository(const std::string& reponame);
    Repository& getRepository(const std::string& name);

private:
    DataRepoManager();

    std::mutex mMutex;
    State mState;
    std::unordered_map<std::string, Repository*> mRepos;
};

}

#endif