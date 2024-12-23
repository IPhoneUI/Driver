#ifndef UTILS_VARIANT_H
#define UTILS_VARIANT_H

#include <unordered_map>
#include "filesystem.h"

namespace utils {
    
class Variant
{
public:
    enum Type {
        Unknown,
        Integer,
        Boolean,
        String,
        Double
    };

    Variant() {}

    Variant(const nlohmann::json& jsonVal);

    Variant(const Variant &param);

    Variant(Variant &&param);

    Variant &operator=(const Variant &param);

    Variant &operator=(Variant &&param);

    template <typename T>
    Variant &operator=(const T &value);

    template <typename T>
    void set(const T& value)
    {
        mJsonVal = value;
    }

    template <typename T>
    T get() const
    {
        return mJsonVal.get<T>();
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
    nlohmann::json mJsonVal;
};

template <typename T, typename std::enable_if<std::is_same<T, std::string>::value, bool>::type = true>
inline Variant::operator T() const
{
    return mJsonVal.get<std::string>();
}

template <typename T, typename std::enable_if<std::is_same<T, int>::value, bool>::type = true>
inline Variant::operator T() const
{
    return mJsonVal.get<int>();
}

template <typename T, typename std::enable_if<std::is_same<T, bool>::value, bool>::type = true>
inline Variant::operator T() const
{
    return mJsonVal.get<bool>();
}

template <typename T, typename std::enable_if<std::is_same<T, double>::value, bool>::type = true>
inline Variant::operator T() const
{
    return mJsonVal.get<double>();
}

}

#endif