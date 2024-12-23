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

    Variant(const boost::property_tree::ptree &ptree);

    Variant(const Variant &param);

    Variant(Variant &&param);

    Variant &operator=(const Variant &param);

    Variant &operator=(Variant &&param);

    template <typename T>
    Variant &operator=(const T &value);

    template <typename T>
    T value() const
    {
        return mData.get_value<T>();
    }

    Type type() const 
    {
        const std::vector<std::pair<Type, std::function<void()>>> typeChecks = 
        {
            {Integer, [&]() { mData.get_value<int>(); }},
            {Boolean, [&]() { mData.get_value<bool>(); }},
            {Double, [&]() { mData.get_value<double>(); }},
            {String, [&]() { mData.get_value<std::string>(); }},
        };

        for (const auto& [type, check] : typeChecks) 
        {
            try 
            {
                check();
                return type;
            } 
            catch (const boost::property_tree::ptree_bad_data&) 
            {
                // To do
            }
        }

        throw std::runtime_error("Unsupported type");
    }

    template <typename T>
    void setValue(const T& value)
    {
        mData.put_value(value);
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