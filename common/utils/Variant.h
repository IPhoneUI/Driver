#ifndef UTILS_VARIANT_T_H
#define UTILS_VARIANT_T_H

#include <list>
#include <unordered_map>
#include <utils/filesystem.h>
#include <storages/Parameter.h>

namespace driver::utils {

class Variant {
public: 
    enum Type {
        NoneType,
        ParameterType,
        ParameterListType
    };

    Variant() {}

    Variant(const Variant& other);

    Variant& operator=(const Variant &other);

    Variant(Variant&& other);

    Variant& operator=(Variant &&other);

    Variant(boost::property_tree::ptree ptree);

    Variant(std::list<std::unordered_map<std::string, boost::property_tree::ptree>> ptreeMap);

    Variant(const storages::Parameter& parameter);

    Variant(const std::list<std::unordered_map<std::string, storages::Parameter>>& parameter);

    Type type() const
    {
        return mType;
    }

    std::list<std::unordered_map<std::string, storages::Parameter>> toList() const
    {
        if (mType == ParameterListType)
            return parameters;
        else 
            return {};
    }

    template <typename T>
    operator T() const
    {
        if (mType == ParameterType)
            return parameter.data().get_value<T>();
        else if (mType == ParameterListType)
            return T();
        else 
            return T();
    }

private:
    Type mType {NoneType};
    storages::Parameter parameter;
    std::list<std::unordered_map<std::string, storages::Parameter>> parameters;
};

}

#endif 