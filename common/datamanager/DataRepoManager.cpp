#include "DataRepoManager.h"
#include <utils/Logger.h>
#include <variant>

namespace common {

#define SERVICE_NAME base::utils::highlightString("DataRepoManager").c_str()

DataRepoManager::DataRepoManager()
{
}

DataRepoManager& DataRepoManager::instance()
{
    static DataRepoManager ins;
    return ins;
}

Repository& DataRepoManager::repository(const std::string& name)
{
    static Repository empty;
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
}

void DataRepoManager::push()
{

}

}
