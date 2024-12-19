#include "DataRepoManager.h"
#include <utils/Logger.h>
#include <variant>

namespace common {

#define SERVICE_NAME base::utils::highlightString("DataRepoManager").c_str()

DataRepoManager::DataRepoManager()
{
    Repository* simRepo = addRepository("sim");
    if (simRepo != nullptr)
    {
        simRepo->addParameter("phonenumber", ParameterIndex::SIM_PhoneNumber);
        simRepo->addParameter("network", ParameterIndex::SIM_Network);
        simRepo->addParameter("phonesignal", ParameterIndex::SIM_PhoneSignal);
        simRepo->addParameter("wifipassword", ParameterIndex::SIM_WifiPassword);
        simRepo->addParameter("allowaccess", ParameterIndex::SIM_AllowAccess);
        simRepo->addParameter("cellular", ParameterIndex::SIM_CellularStatus);
        simRepo->addParameter("maxcompatibility", ParameterIndex::SIM_MaxCompatibility);
        simRepo->addParameter("contact", ParameterIndex::SIM_Contact);
        simRepo->addParameter("history", ParameterIndex::SIM_History);
    }

    Repository* flashmemoryRepo = addRepository("flashmemory");
    if (flashmemoryRepo != nullptr)
    {
        flashmemoryRepo->addParameter("recording", ParameterIndex::FMem_Recording);
        flashmemoryRepo->addParameter("delete_recording", ParameterIndex::FMem_DeleteRecording);
        flashmemoryRepo->addParameter("airplane_mode", ParameterIndex::FMem_AirPlaneMode);
    }

    Repository* speakerRepo = addRepository("speaker");
    if (speakerRepo != nullptr)
    {
        speakerRepo->addParameter("muted", ParameterIndex::Speaker_Muted);
        speakerRepo->addParameter("volume", ParameterIndex::Speaker_Volume);
    }

    Repository* wifiRepo = addRepository("wifi");
    if (wifiRepo != nullptr)
    {
        wifiRepo->addParameter("data", ParameterIndex::Wifi_Data);
    }

    setState(WaitToSyncDataState);

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

Repository& DataRepoManager::repository(const std::string& name)
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
