#include "VariantObj.h"

namespace utils {

VariantObj::VariantObj(const std::unordered_map<std::string, Variant> &variantObj)
    : mVariantObj(variantObj)
{
}

VariantObj::VariantObj(const VariantObj &param)
{
    mVariantObj = param.mVariantObj;
}

VariantObj::VariantObj(VariantObj &&param)
{
    mVariantObj = std::move(param.mVariantObj);
}

VariantObj &VariantObj::operator=(const VariantObj &param)
{
    if (this != &param)
    {
        mVariantObj = param.mVariantObj;
    }
    return *this;
}

VariantObj &VariantObj::operator=(VariantObj &&param)
{
    if (this != &param)
    {
        mVariantObj = std::move(param.mVariantObj);
    }
    return *this;
}

VariantObj &VariantObj::operator=(const std::unordered_map<std::string, utils::Variant>& variantObj)
{
    mVariantObj = std::move(variantObj);
    return *this;
}

}