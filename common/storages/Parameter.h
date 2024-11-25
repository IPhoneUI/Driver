#ifndef PARAMETER_H
#define PARAMETER_H

#include <unordered_map>
#include <utils/filesystem.h>

namespace driver::storages {

class Parameter
{
public:
    Parameter() {}

    Parameter(const boost::property_tree::ptree& ptree);
    
    Parameter(const Parameter &param);

    Parameter(Parameter &&param);

    Parameter &operator=(const Parameter &param);
    
    Parameter &operator=(Parameter &&param);

    boost::property_tree::ptree data() const;

    template <typename T>
    operator T() const
    {
        return mData.get_value<T>();
    }

private:
    boost::property_tree::ptree mData;
};

}

#endif