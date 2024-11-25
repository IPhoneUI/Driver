#include "Variant.h"
#include <any>

namespace driver::utils {

Variant::Variant(const storages::Parameter &value)
    : mType(ParameterType)
{
    parameter = value;
}

Variant::Variant(const std::list<std::unordered_map<std::string, storages::Parameter>> &value)
    : mType(ParameterListType)
{
    parameters = value;
}

Variant::Variant(const Variant &other)
{
    parameters = other.parameters;
    parameter = other.parameter;
    mType = other.mType;
}

Variant::Variant(Variant &&other)
{
    parameters = std::move(other.parameters);
    parameter = std::move(other.parameter);
    mType = std::move(other.mType);
}

Variant &Variant::operator=(const Variant &other)
{
    parameters = other.parameters;
    parameter = other.parameter;
    mType = other.mType;

    return *this;
}

Variant &Variant::operator=(Variant &&other)
{
    parameters = std::move(other.parameters);
    parameter = std::move(other.parameter);
    mType = std::move(other.mType);

    return *this;
}

Variant::Variant(boost::property_tree::ptree ptree)
    : mType(ParameterType)
{
    parameter = storages::Parameter(ptree);
}

Variant::Variant(std::list<std::unordered_map<std::string, boost::property_tree::ptree>> ptreeMap)
    : mType(ParameterListType)
{
    for (const auto &param_map : ptreeMap)
    {
        std::unordered_map<std::string, storages::Parameter> temp_map;
        for (const auto &pair : param_map)
        {
            temp_map.emplace(pair.first, storages::Parameter(pair.second));
        }
        parameters.push_back(temp_map);
    }
}

}