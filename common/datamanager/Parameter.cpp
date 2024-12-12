#include "Parameter.h"

namespace common {

Parameter::Parameter(const boost::property_tree::ptree& ptree)
    : mData(ptree)
{
}

Parameter::Parameter(const Parameter &param)
{
    mData = param.mData;
}

Parameter::Parameter(Parameter &&param)
{
    mData = std::move(param.mData);
}

Parameter &Parameter::operator=(const Parameter &param)
{
    if (this != &param)
    {
        mData = param.mData;
    }
    return *this;
}

Parameter &Parameter::operator=(Parameter &&param)
{
    if (this != &param)
    {
        mData = std::move(param.mData);
    }
    return *this;
}

boost::property_tree::ptree Parameter::data() const
{
    return mData;
}

}