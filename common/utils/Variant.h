#ifndef UTILS_VARIANT_H
#define UTILS_VARIANT_H

#include <unordered_map>
#include "filesystem.h"

namespace utils {
    
class Variant
{
public:
    enum type_t {
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

    type_t type() const 
    {
        if (mJsonVal.is_boolean())
            return Boolean;
        else if (mJsonVal.is_string())
            return String;
        else if (mJsonVal.is_number_integer())
            return Integer;
        else if (mJsonVal.is_number_float())
            return Double;
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
    type_t mType;
};

template <typename T, typename std::enable_if<std::is_same<T, std::string>::value, bool>::type>
inline Variant::operator T() const
{
    return mJsonVal.get<std::string>();
}

template <typename T, typename std::enable_if<std::is_same<T, int>::value, bool>::type>
inline Variant::operator T() const
{
    return mJsonVal.get<int>();
}

template <typename T, typename std::enable_if<std::is_same<T, bool>::value, bool>::type>
inline Variant::operator T() const
{
    return mJsonVal.get<bool>();
}

template <typename T, typename std::enable_if<std::is_same<T, double>::value, bool>::type>
inline Variant::operator T() const
{
    return mJsonVal.get<double>();
}

}

#endif