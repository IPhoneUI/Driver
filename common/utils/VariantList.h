#ifndef UTILS_VARIANTLIST_H
#define UTILS_VARIANTLIST_H

#include <unordered_map>
#include <list>
#include "filesystem.h"
#include "Variant.h"

namespace utils {

class VariantList
{
public:
    VariantList() {}

    VariantList(const std::list<std::unordered_map<std::string, Variant>> &variantList);

    VariantList(const VariantList &param);

    VariantList(VariantList &&param);

    VariantList &operator=(const VariantList &param);

    VariantList &operator=(VariantList &&param);

    void push(const std::unordered_map<std::string, Variant> &item);

    auto begin() 
    { 
        return mVariantList.begin(); 
    }

    auto end() 
    { 
        return mVariantList.end(); 
    }
    
    auto begin() const 
    { 
        return mVariantList.begin(); 
    }

    auto end() const 
    { 
        return mVariantList.end(); 
    }

private:
    std::list<std::unordered_map<std::string, Variant>> mVariantList;
};

}

#endif