#include "DataRepoManager.h"
#include <utils/Logger.h>
#include <variant>

namespace driver::storages {

#define SERVICE_NAME highlightString("DataRepoManager").c_str()

DataRepoManager::DataRepoManager()
{
    Repository* simRepo = addRepository("SIM");
    if (simRepo != nullptr)
    {
        simRepo->addParameter("phonenumber", ParameterIndex::SIM_PhoneNumber);
        simRepo->addParameter("network", ParameterIndex::SIM_Network);
        simRepo->addParameter("phonesignal", ParameterIndex::SIM_PhoneSignal);
        simRepo->addParameter("wifipassword", ParameterIndex::SIM_WifiPassword);
        simRepo->addParameter("allowaccess", ParameterIndex::SIM_AllowAccess);
        simRepo->addParameter("cellular", ParameterIndex::SIM_CellularStatus);
        simRepo->addParameter("maxcompatibility", ParameterIndex::SIM_MaxCompatibility);
    }

    // add parameter for Cloud
    Repository* cloudRepo = addRepository("Cloud");
    if (cloudRepo != nullptr)
    {
        cloudRepo->addParameter("contact", ParameterIndex::Cloud_Contact);
        cloudRepo->addParameter("history", ParameterIndex::Cloud_History);
        cloudRepo->addParameter("recording", ParameterIndex::Cloud_Recording);
        cloudRepo->addParameter("delete_recording", ParameterIndex::Cloud_DeleteRecording);
    }

    // add parameter for Wifi
    Repository* wifiRepo = addRepository("Wifi");
    if (wifiRepo != nullptr)
    {
        wifiRepo->addParameter("data", ParameterIndex::Wifi_Data);
    }


    setState(WaitToSyncDataState);

    // Fix later;
    pull();
}

DataRepoManager& DataRepoManager::instance()
{
    static DataRepoManager ins;
    return ins;
}

void DataRepoManager::setState(State state)
{
    if (mState == state)
        return;
    
    mState = state;
}

Repository& DataRepoManager::getRepository(const std::string& name)
{
    static Repository empty;
    if (mState != ReadyState)
        return empty;

    auto it = mRepos.find(name);

    if (it != mRepos.end())
    {
        return (*(*it).second);
    }

    return empty;
}

Repository* DataRepoManager::addRepository(const std::string& repoName)
{
    Repository* repo = nullptr;
    for (auto it = mRepos.begin(); it != mRepos.end(); ++it)
    {
        if (it->first == repoName)
            return repo;
    }

    repo = new Repository(repoName);
    mRepos.emplace(repoName, repo);
    
    return repo;

}

void DataRepoManager::pull()
{
    std::lock_guard<std::mutex> lock(mMutex);
    for (auto it = mRepos.begin(); it != mRepos.end(); ++it)
    {
        (*it).second->pull();
    }

    setState(ReadyState);
}

void DataRepoManager::push()
{

}

}
