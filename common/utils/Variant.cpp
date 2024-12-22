#include "Variant.h"

namespace utils {

Variant::Variant(const boost::property_tree::ptree &ptree)
    : mData(ptree)
{
}

Variant::Variant(const Variant &param)
{
    mData = param.mData;
}

Variant::Variant(Variant &&param)
{
    mData = std::move(param.mData);
}

Variant &Variant::operator=(const Variant &param)
{
    if (this != &param)
    {
        mData = param.mData;
    }
    return *this;
}

Variant &Variant::operator=(Variant &&param)
{
    if (this != &param)
    {
        mData = std::move(param.mData);
    }
    return *this;
}

template <typename T>
Variant &Variant::operator=(const T &value)
{
    setValue(value);
    return *this;
}

template <>
Variant& Variant::operator=<bool>(const bool& value)
{
    setValue(value);
    return *this;
}

template <>
Variant& Variant::operator=<double>(const double& value)
{
    setValue(value);
    return *this;
}

template <>
Variant& Variant::operator=<int>(const int& value)
{
    setValue(value);
    return *this;
}

template <>
Variant& Variant::operator=<std::string>(const std::string& value)
{
    setValue(value);
    return *this;
}

}