#ifndef UTILS_PARAMETER_H
#define UTILS_PARAMETER_H

#include <list>
#include <unordered_map>
#include <utils/filesystem.h>
#include <datamanager/PTree.h>

namespace common {

class Parameter {
public: 
    enum Type {
        NoneType,
        ParameterType,
        ParameterListType
    };

    Parameter() {}

    Parameter(const Parameter& other);

    Parameter& operator=(const Parameter &other);

    Parameter(Parameter&& other);

    Parameter& operator=(Parameter &&other);

    Parameter(boost::property_tree::ptree ptree);

    Parameter(std::list<std::unordered_map<std::string, boost::property_tree::ptree>> ptreeMap);

    Parameter(const PTree& parameter);

    Parameter(const std::list<std::unordered_map<std::string, PTree>>& parameter);

    Type type() const
    {
        return mType;
    }

    std::list<std::unordered_map<std::string, PTree>> toList() const
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
    PTree parameter;
    std::list<std::unordered_map<std::string, PTree>> parameters;
};

}

#endif 