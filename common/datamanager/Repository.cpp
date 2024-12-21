#include "Repository.h"
#include <utils/Logger.h>

namespace common {

#define SERVICE_NAME base::utils::highlightString("Repository").c_str()

Repository::Repository(const std::string& name) :
    mName{name}
{
    mPath = filesystem::parent_path(__FILE__) + "/database/" + mName + ".json";
}

Parameter Repository::operator[](ParameterIndex index)
{
    if (index < 0 || index >= ParameterIndex::ParameterMax)
        return Parameter();

    std::shared_lock<std::shared_mutex> lock(mMutex);
    for (auto it = mConfigParameters.begin(); it != mConfigParameters.end(); ++it)
    {
        if ((*it)->index == static_cast<int>(index))
        {
            return Parameter((*it)->value);
        }
    }

    return Parameter();
}

void Repository::addParameter(const std::string& keyName, ParameterIndex index)
{
    ConfigParameter* param = findParameter(keyName);

    if (param != nullptr)
    {
        LOG_WARN("Variant %s is exists! addParameter is failed [%s]", keyName, SERVICE_NAME);
        return;
    }

    param = new ConfigParameter(keyName, index);
    mConfigParameters.push_back(param);
}


ConfigParameter* Repository::findParameter(const std::string& name)
{
    for (auto it = mConfigParameters.begin(); it != mConfigParameters.end(); ++it)
    {
        if ((*it)->name == name)
        {
            return (*it);
        }
    }

    return nullptr;
}

bool Repository::pull()
{
    try 
    {
        std::unique_lock<std::shared_mutex> lock(mMutex);

        setState(WaitToPullCompleted);
        boost::property_tree::ptree ptree;

        boost::property_tree::read_json(mPath, ptree);
        for (const auto &field : ptree) 
        {
            ConfigParameter* parameter = findParameter(field.first);
            if (parameter != nullptr)
            {
                if (field.second.empty())
                {
                    parameter->value = Parameter(field.second);
                }
                else 
                {
                    if (ptree.count(field.first) == 0)
                        throw std::runtime_error("JSON does not contain 'data' key");
                        
                    std::list<std::unordered_map<std::string, boost::property_tree::ptree>> ptreeMap;
                    for (const auto &item : ptree.get_child(field.first))
                    {
                        std::unordered_map<std::string, boost::property_tree::ptree> dataMap;

                        for (const auto &field : item.second)
                        {
                            dataMap.emplace(field.first, field.second);
                        }

                        ptreeMap.push_back(dataMap);
                    }

                    parameter->value = Parameter(ptreeMap);
                }
            }
        }
        lock.unlock();
        setState(PullCompleted);
    } 
    catch (const boost::property_tree::json_parser_error &e) 
    {
        LOG_ERR("JSON parsing error: %s", e.what());
        setState(PullError);
    } 
    catch (const std::runtime_error &e) 
    {
        LOG_ERR("Runtime error: %s", e.what());
        setState(PullError);
    } 
    catch (const std::exception &e) 
    {
        LOG_ERR("Exception: %s", e.what());
        setState(PullError);
    }

    return true;
}

void Repository::setState(State value)
{
    if (mState == value)
        return;

    mState = value;
    LOG_INFO("Repository [%s] change state to %d", mName.c_str(), (int)mState);
    onRepoStateChanged.emit(mState);
}

}