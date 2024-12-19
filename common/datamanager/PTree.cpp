#include "PTree.h"

namespace common {

PTree::PTree(const boost::property_tree::ptree& ptree)
    : mData(ptree)
{
}

PTree::PTree(const PTree &param)
{
    mData = param.mData;
}

PTree::PTree(PTree &&param)
{
    mData = std::move(param.mData);
}

PTree &PTree::operator=(const PTree &param)
{
    if (this != &param)
    {
        mData = param.mData;
    }
    return *this;
}

PTree &PTree::operator=(PTree &&param)
{
    if (this != &param)
    {
        mData = std::move(param.mData);
    }
    return *this;
}

boost::property_tree::ptree PTree::data() const
{
    return mData;
}

}