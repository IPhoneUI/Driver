#include "Parameter.h"
#include <any>

namespace common {

Parameter::Parameter(const PTree &value)
    : mType(ParameterType)
{
    parameter = value;
}

Parameter::Parameter(const std::list<std::unordered_map<std::string, PTree>> &value)
    : mType(ParameterListType)
{
    parameters = value;
}

Parameter::Parameter(const Parameter &other)
{
    parameters = other.parameters;
    parameter = other.parameter;
    mType = other.mType;
}

Parameter::Parameter(Parameter &&other)
{
    parameters = std::move(other.parameters);
    parameter = std::move(other.parameter);
    mType = std::move(other.mType);
}

Parameter &Parameter::operator=(const Parameter &other)
{
    parameters = other.parameters;
    parameter = other.parameter;
    mType = other.mType;

    return *this;
}

Parameter &Parameter::operator=(Parameter &&other)
{
    parameters = std::move(other.parameters);
    parameter = std::move(other.parameter);
    mType = std::move(other.mType);

    return *this;
}

Parameter::Parameter(boost::property_tree::ptree ptree)
    : mType(ParameterType)
{
    parameter = PTree(ptree);
}

Parameter::Parameter(std::list<std::unordered_map<std::string, boost::property_tree::ptree>> ptreeMap)
    : mType(ParameterListType)
{
    for (const auto &param_map : ptreeMap)
    {
        std::unordered_map<std::string, PTree> temp_map;
        for (const auto &pair : param_map)
        {
            temp_map.emplace(pair.first, PTree(pair.second));
        }
        parameters.push_back(temp_map);
    }
}

}