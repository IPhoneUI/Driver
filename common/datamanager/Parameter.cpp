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

Parameter::Parameter(const nlohmann::json& jsonVal)
{
    if (jsonVal.is_array()) 
    {
        mType = VariantListType;
        for (const auto& item : jsonVal) 
        {
            if (item.is_object()) 
            {
                std::unordered_map<std::string, utils::Variant> itemMap;
                for (const auto& [key, val] : item.items()) 
                {
                    itemMap[key] = utils::Variant(val);
                }
                mVariantList.push(itemMap);
            }
        }
    } 
    else if (jsonVal.is_string() || jsonVal.is_number_integer() 
            || jsonVal.is_boolean() || jsonVal.is_number_float()) 
    {
        mType = VariantType;
        mVariant = utils::Variant(jsonVal);
    }
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

Parameter &Parameter::operator=(const utils::VariantList &variantList)
{
    mVariantList = variantList;

    return *this;
}

template <typename T>
Parameter &Parameter::operator=(const T& value)
{
    mVariant.set(value);
    return *this;
}

template <>
Parameter& Parameter::operator=<bool>(const bool& value)
{
    mVariant.set(value);
    return *this;
}

template <>
Parameter& Parameter::operator=<double>(const double& value)
{
    mVariant.set(value);
    return *this;
}

template <>
Parameter& Parameter::operator=<int>(const int& value)
{
    mVariant.set(value);
    return *this;
}

template <>
Parameter& Parameter::operator=<std::string>(const std::string& value)
{
    mVariant.set(value);
    return *this;
}

}