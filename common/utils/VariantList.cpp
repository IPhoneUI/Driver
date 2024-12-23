#include "VariantList.h"

namespace utils {

VariantList::VariantList(const std::list<std::unordered_map<std::string, Variant>> &variantList)
    : mVariantList(variantList)
{
}

VariantList::VariantList(const VariantList &param)
{
    mVariantList = param.mVariantList;
}

VariantList::VariantList(VariantList &&param)
{
    mVariantList = std::move(param.mVariantList);
}

VariantList &VariantList::operator=(const VariantList &param)
{
    if (this != &param)
    {
        mVariantList = param.mVariantList;
    }
    return *this;
}

VariantList &VariantList::operator=(VariantList &&param)
{
    if (this != &param)
    {
        mVariantList = std::move(param.mVariantList);
    }
    return *this;
}

void VariantList::push(const std::unordered_map<std::string, Variant> &item)
{
    mVariantList.push_back(item);
}

}