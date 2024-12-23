#include "Variant.h"

namespace utils {

Variant::Variant(const nlohmann::json& jsonVal)
    : mJsonVal(jsonVal)
{
}

Variant::Variant(const Variant &param)
{
    mJsonVal = param.mJsonVal;
}

Variant::Variant(Variant &&param)
{
    mJsonVal = std::move(param.mJsonVal);
}

Variant &Variant::operator=(const Variant &param)
{
    if (this != &param)
    {
        mJsonVal = param.mJsonVal;
    }
    return *this;
}

Variant &Variant::operator=(Variant &&param)
{
    if (this != &param)
    {
        mJsonVal = std::move(param.mJsonVal);
    }
    return *this;
}

template <typename T>
Variant &Variant::operator=(const T &value)
{
    set(value);
    return *this;
}

template <>
Variant& Variant::operator=<bool>(const bool& value)
{
    set(value);
    return *this;
}

template <>
Variant& Variant::operator=<double>(const double& value)
{
    set(value);
    return *this;
}

template <>
Variant& Variant::operator=<int>(const int& value)
{
    set(value);
    return *this;
}

template <>
Variant& Variant::operator=<std::string>(const std::string& value)
{
    set(value);
    return *this;
}

}