#ifndef UTILS_PTREE_H
#define UTILS_PTREE_H

#include <unordered_map>
#include <utils/filesystem.h>

namespace common {

class PTree
{
public:
    PTree() {}

    PTree(const boost::property_tree::ptree& ptree);
    
    PTree(const PTree &param);

    PTree(PTree &&param);

    PTree &operator=(const PTree &param);
    
    PTree &operator=(PTree &&param);

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