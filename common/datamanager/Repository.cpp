#include "Repository.h"
#include <utils/Logger.h>

namespace common {

#define SERVICE_NAME base::utils::highlightString("Repository").c_str()

void Repository::setName(const std::string& name)
{
    mName = name;
    mPath = filesystem::parent_path(__FILE__) + "/database/" + mName + ".json";
}

Parameter& Repository::operator[](ParameterIndex index)
{
    static Parameter invalidParam; 
    if (mName.empty() || mPath.empty())
    {
        LOG_WARN("repo name or path is empty [%s]", SERVICE_NAME);
        return invalidParam;
    }
    
    if (index < 0 || index >= ParameterIndex::ParameterMax)
        return invalidParam;

    std::shared_lock<std::shared_mutex> lock(mMutex);
    for (auto it = mConfigParameters.begin(); it != mConfigParameters.end(); ++it)
    {
        if ((*it)->index == static_cast<int>(index))
        {
            return (*it)->value;
        }
    }

    return invalidParam;
}

void Repository::addParam(const std::string& keyName, ParameterIndex index)
{
    if (mName.empty() || mPath.empty())
    {
        LOG_WARN("repo name or path is empty [%s]", SERVICE_NAME);
        return;
    }

    ConfigParameter* param = findParam(keyName);

    if (param != nullptr)
    {
        LOG_WARN("Variant %s is exists! addParam is failed [%s]", keyName, SERVICE_NAME);
        return;
    }

    param = new ConfigParameter(keyName, index);
    mConfigParameters.push_back(param);
}


ConfigParameter* Repository::findParam(const std::string& name)
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

void Repository::pull()
{
    if (mName.empty() || mPath.empty())
    {
        LOG_WARN("repo name or path is empty [%s]", SERVICE_NAME);
        setState(PullError);
        return;
    }

    std::unique_lock<std::shared_mutex> lock(mMutex);
    setState(WaitToPullCompleted);
    std::ifstream input_file(mPath);
    if (!input_file.is_open()) 
    {
        setState(PullError);
        return;
    }

    json j;
    input_file >> j;

    for (auto it = j.begin(); it != j.end(); ++it) 
    {
        const std::string& key = it.key();
        const nlohmann::json& value = it.value();
        ConfigParameter* configParam = findParam(key);
        if (configParam != nullptr)
        {
            configParam->value = Parameter(value);
        }
    }
    lock.unlock();
    setState(PullCompleted);
}

void Repository::push()
{
    if (mName.empty() || mPath.empty())
    {
        LOG_WARN("repo name or path is empty [%s]", SERVICE_NAME);
        setState(PushError);
        return;
    }
    LOG_INFO("repo: %s", mName.c_str());
    std::unique_lock<std::shared_mutex> lock(mMutex);

    json j;

    for (const auto& configParam : mConfigParameters) 
    {
        if (configParam->value.type() == Parameter::VariantType)
        {
            utils::Variant value = configParam->value;
            switch (value.type()) {
            case utils::Variant::String:
                j[configParam->name] = value.get<std::string>(); 
                break;
            case utils::Variant::Integer:
                j[configParam->name] = value.get<int>(); 
                break;
            case utils::Variant::Boolean:
                j[configParam->name] = value.get<bool>(); 
                break;
            case utils::Variant::Double:
                j[configParam->name] = value.get<double>(); 
                break;
            }
            
        }
        else if (configParam->value.type() == Parameter::VariantObjType)
        {
            utils::VariantObj value = configParam->value;
            std::unordered_map<std::string, utils::Variant> data = value;
            json obj;
            for (const auto& [subKey, subValue] : data)
            {
                switch (subValue.type()) {
                case utils::Variant::String:
                    obj[subKey] = subValue.get<std::string>(); 
                    break;
                case utils::Variant::Integer:
                    obj[subKey] = subValue.get<int>(); 
                    break;
                case utils::Variant::Boolean:
                    obj[subKey] = subValue.get<bool>(); 
                    break;
                case utils::Variant::Double:
                    obj[subKey] = subValue.get<double>(); 
                    break;
                }
            }
            j[configParam->name] = obj;
            
        }
        else if (configParam->value.type() == Parameter::VariantListType)
        {
            json array = json::array();
            utils::VariantList variantList = configParam->value; 
            for (const auto& item : variantList)
            {
                json obj;
                for (const auto& [subKey, subValue] : item)
                {
                    switch (subValue.type()) {
                    case utils::Variant::String:
                        obj[subKey] = subValue.get<std::string>(); 
                        break;
                    case utils::Variant::Integer:
                        obj[subKey] = subValue.get<int>(); 
                        break;
                    case utils::Variant::Boolean:
                        obj[subKey] = subValue.get<bool>(); 
                        break;
                    case utils::Variant::Double:
                        obj[subKey] = subValue.get<double>(); 
                        break;
                    }
                }
                array.push_back(obj);
            }
            j[configParam->name] = array;
        }
    }

    std::ofstream output_file(mPath);
    if (!output_file.is_open())
    {
        setState(PushError);
        return;
    }

    output_file << j.dump(4);
    lock.unlock();
    setState(PushCompleted);
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