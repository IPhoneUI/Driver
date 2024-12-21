#include "Variant.h"

namespace utils {

Variant::Variant(const boost::property_tree::ptree &ptree)
    : mData(ptree)
{
}

Variant::Variant(const Variant &param)
{
    mData = param.mData;
}

Variant::Variant(Variant &&param)
{
    mData = std::move(param.mData);
}

Variant &Variant::operator=(const Variant &param)
{
    if (this != &param)
    {
        mData = param.mData;
    }
    return *this;
}

Variant &Variant::operator=(Variant &&param)
{
    if (this != &param)
    {
        mData = std::move(param.mData);
    }
    return *this;
}

}