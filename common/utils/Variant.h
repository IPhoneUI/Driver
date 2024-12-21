#ifndef UTILS_VARIANT_H
#define UTILS_VARIANT_H

#include <unordered_map>
#include "filesystem.h"

namespace utils {
    
class Variant
{
public:
    Variant() {}

    Variant(const boost::property_tree::ptree &ptree);

    Variant(const Variant &param);

    Variant(Variant &&param);

    Variant &operator=(const Variant &param);

    Variant &operator=(Variant &&param);

    template <typename T>
    T value() const
    {
        return mData.get_value<T>();
    }

    template <typename T, typename std::enable_if<std::is_same<T, std::string>::value, bool>::type = true>
    operator T() const;

    template <typename T, typename std::enable_if<std::is_same<T, int>::value, bool>::type = true>
    operator T() const;

    template <typename T, typename std::enable_if<std::is_same<T, bool>::value, bool>::type = true>
    operator T() const;

    template <typename T, typename std::enable_if<std::is_same<T, double>::value, bool>::type = true>
    operator T() const;

private:
    boost::property_tree::ptree mData;
};

template <typename T, typename std::enable_if<std::is_same<T, std::string>::value, bool>::type = true>
inline Variant::operator T() const
{
    return mData.get_value<std::string>();
}

template <typename T, typename std::enable_if<std::is_same<T, int>::value, bool>::type = true>
inline Variant::operator T() const
{
    return mData.get_value<int>();
}

template <typename T, typename std::enable_if<std::is_same<T, bool>::value, bool>::type = true>
inline Variant::operator T() const
{
    return mData.get_value<bool>();
}

template <typename T, typename std::enable_if<std::is_same<T, double>::value, bool>::type = true>
inline Variant::operator T() const
{
    return mData.get_value<double>();
}

}

#endif