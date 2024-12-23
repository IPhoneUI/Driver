#ifndef UTILS_PARAMETER_H
#define UTILS_PARAMETER_H

#include <list>
#include <unordered_map>
#include <utils/filesystem.h>
#include <utils/Variant.h>
#include <utils/VariantList.h>

namespace common {

class Parameter
{
public:
    enum Type
    {
        Unknown,
        VariantType,
        VariantListType
    };

    Parameter() {}

    Parameter(const Parameter &other);

    Parameter &operator=(const Parameter &other);

    Parameter(Parameter &&other);

    Parameter &operator=(Parameter &&other);

    template <typename T>
    Parameter &operator=(const T& value);

    Parameter &operator=(const utils::VariantList &variantList);

    Parameter(boost::property_tree::ptree ptree);

    Parameter(std::list<std::unordered_map<std::string, boost::property_tree::ptree>> ptreeMap);

    Parameter(const utils::Variant &variant);

    Parameter(const utils::VariantList &variantList);

    template <typename T, typename std::enable_if<std::is_same<T, utils::Variant>::value, bool>::type = true>
    operator T() const;

    template <typename T, typename std::enable_if<std::is_same<T, utils::VariantList>::value, bool>::type = true>
    operator T() const;

    template <typename T, typename std::enable_if<std::is_same<T, bool>::value, bool>::type = true>
    operator T() const;

    template <typename T, typename std::enable_if<std::is_same<T, std::string>::value, bool>::type = true>
    operator T() const;

    template <typename T, typename std::enable_if<std::is_same<T, double>::value, bool>::type = true>
    operator T() const;

    template <typename T, typename std::enable_if<std::is_same<T, int>::value, bool>::type = true>
    operator T() const;

    Type type() const
    {
        return mType;
    }

private:
    Type mType{Unknown};
    utils::Variant mVariant;
    utils::VariantList mVariantList;
};

template <typename T, typename std::enable_if<std::is_same<T, utils::Variant>::value, bool>::type = true>
inline Parameter::operator T() const
{
    if (mType == VariantType)
        return mVariant;
    throw std::bad_cast();
}

template <typename T, typename std::enable_if<std::is_same<T, utils::VariantList>::value, bool>::type = true>
inline Parameter::operator T() const
{
    if (mType == VariantListType)
        return mVariantList;
    throw std::bad_cast();
}

template <typename T, typename std::enable_if<std::is_same<T, bool>::value, bool>::type = true>
inline Parameter::operator T() const
{
    if (mType == VariantType)
        return mVariant.value<bool>();
    throw std::bad_cast();
}

template <typename T, typename std::enable_if<std::is_same<T, std::string>::value, bool>::type = true>
inline Parameter::operator T() const
{
    if (mType == VariantType)
        return mVariant.value<std::string>();
    throw std::bad_cast();
}

template <typename T, typename std::enable_if<std::is_same<T, double>::value, bool>::type = true>
inline Parameter::operator T() const
{
    if (mType == VariantType)
        return mVariant.value<double>();
    throw std::bad_cast();
}

template <typename T, typename std::enable_if<std::is_same<T, int>::value, bool>::type = true>
inline Parameter::operator T() const
{
    if (mType == VariantType)
        return mVariant.value<int>();
    throw std::bad_cast();
}

}

#endif