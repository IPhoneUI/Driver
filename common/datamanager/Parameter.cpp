#include "Parameter.h"
#include <any>

namespace common {

Parameter::Parameter(const utils::Variant &variant)
    : mType(VariantType)
{
    mVariant = variant;
}

Parameter::Parameter(const utils::VariantList &variantList)
    : mType(VariantListType)
{
    mVariantList = variantList;
}

Parameter::Parameter(const Parameter &other)
{
    mVariantList = other.mVariantList;
    mVariant = other.mVariant;
    mType = other.mType;
}

Parameter::Parameter(Parameter &&other)
{
    mVariantList = std::move(other.mVariantList);
    mVariant = std::move(other.mVariant);
    mType = std::move(other.mType);
}

Parameter &Parameter::operator=(const Parameter &other)
{
    mVariantList = other.mVariantList;
    mVariant = other.mVariant;
    mType = other.mType;

    return *this;
}

Parameter &Parameter::operator=(Parameter &&other)
{
    mVariantList = std::move(other.mVariantList);
    mVariant = std::move(other.mVariant);
    mType = std::move(other.mType);

    return *this;
}

template <typename T>
Parameter &Parameter::operator=(const T& value)
{
    mVariant.setValue(value);
    return *this;
}

template <>
Parameter& Parameter::operator=<bool>(const bool& value)
{
    mVariant.setValue(value);
    return *this;
}

template <>
Parameter& Parameter::operator=<double>(const double& value)
{
    mVariant.setValue(value);
    return *this;
}

template <>
Parameter& Parameter::operator=<int>(const int& value)
{
    mVariant.setValue(value);
    return *this;
}

template <>
Parameter& Parameter::operator=<std::string>(const std::string& value)
{
    mVariant.setValue(value);
    return *this;
}

Parameter::Parameter(boost::property_tree::ptree ptree)
    : mType(VariantType)
{
    mVariant = utils::Variant(ptree);
}

Parameter::Parameter(std::list<std::unordered_map<std::string, boost::property_tree::ptree>> ptreeMap)
    : mType(VariantListType)
{
    for (const auto &param_map : ptreeMap)
    {
        std::unordered_map<std::string, utils::Variant> temp_map;
        for (const auto &pair : param_map)
        {
            temp_map.emplace(pair.first, utils::Variant(pair.second));
        }
        mVariantList.push(temp_map);
    }
}

}