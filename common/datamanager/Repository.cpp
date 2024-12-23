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

void Repository::writeJson(const boost::property_tree::ptree& ptree) 
{
    std::ofstream file(mPath);
    if (!file.is_open())
        throw std::runtime_error("Could not open file to write JSON");

    file << "{\n";
    bool firstElement = true;
    for (const auto& [key, value] : ptree) 
    {
        if (!firstElement) 
        {
            file << ",\n";
        }
        firstElement = false;

        LOG_INFO("key: %s", key.c_str());
        file << "    \"" << key << "\": ";

        if (value.empty()) {
            try {
                int intValue = value.get_value<int>();
                file << intValue;
                continue;
            } catch (...) {}

            try {
                bool boolValue = value.get_value<bool>();
                file << (boolValue ? "true" : "false");
                continue;
            } catch (...) {}

            try {
                double doubleValue = value.get_value<double>();
                file << doubleValue;
                continue;
            } catch (...) {}

            file << "\"" << value.get_value<std::string>() << "\"";
        } else {
            std::ostringstream nestedStream;
            boost::property_tree::write_json(nestedStream, value, false);
            std::string nestedJson = nestedStream.str();
            file << nestedJson.substr(0, nestedJson.size() - 1);
        }
    }
    file << "\n}\n";

    file.close();
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
    if (mName.empty() || mPath.empty()) {
        LOG_WARN("repo name or path is empty [%s]", SERVICE_NAME);
        setState(PushError);
        return;
    }

    try {
        setState(WaitToPushCompleted);
        std::unique_lock<std::shared_mutex> lock(mMutex);

        boost::property_tree::ptree ptree;
        std::ofstream file(mPath);
        if (!file.is_open())
            throw std::runtime_error("Could not open file to write JSON");

        file << "{\n";
        bool firstElement = true;

        for (const auto& parameter : mConfigParameters) 
        {
            if (!firstElement) 
            {
                file << ",\n";
            }
            firstElement = false;
            file << "    \"" << parameter->name << "\": ";
            // if (parameter->value.type() == Parameter::VariantListType) 
            // {
            //     // boost::property_tree::ptree listNode;

            //     // auto dataList = parameter->value.toList();
            //     // for (const auto& dataMap : dataList) {
            //     //     boost::property_tree::ptree mapNode;
            //     //     for (const auto& [key, value] : dataMap) {
            //     //         mapNode.put(key, value.get_value<std::string>());
            //     //     }
            //     //     listNode.push_back(std::make_pair("", mapNode));
            //     // }
            //     // ptree.add_child(parameter->name, listNode);
            // } 
            // else if (parameter->value.type() == Parameter::VariantType)
            // {
            //     utils::Variant variant = parameter->value;
            //     auto type = variant.type();
            //     LOG_INFO("type: %d", type);
            //     switch (type) {
            //     case utils::Variant::Integer: {
            //         file << variant.value<int>();
            //         break;
            //     }
            //     case utils::Variant::Boolean: {
            //         file << (variant.value<bool>() ? "true" : "false");
            //         break;
            //     }
            //     case utils::Variant::String: {
            //         file << "\"" << variant.value<std::string>() << "\"";
            //         break;
            //     }
            //     case utils::Variant::Double: {
            //         file << variant.value<double>();
            //         break;
            //     }
            //     }
            // }
        }

        file << "\n}\n";
        file.close();

        setState(PushCompleted);

        LOG_INFO("Push data to JSON file completed [%s]", mPath.c_str());
    } 
    catch (const boost::property_tree::json_parser_error& e) 
    {
        LOG_ERR("JSON parsing error during push: %s", e.what());
        setState(PushError);
    } 
    catch (const std::exception& e) 
    {
        LOG_ERR("Exception during push: %s", e.what());
        setState(PushError);
    }
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