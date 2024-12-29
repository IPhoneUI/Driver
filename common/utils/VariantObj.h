#ifndef UTILS_VARIANTOBJ_H
#define UTILS_VARIANTOBJ_H

#include <unordered_map>
#include <list>
#include "filesystem.h"
#include "Variant.h"

namespace utils {

class VariantObj
{
public:
    VariantObj() {}

    VariantObj(const std::unordered_map<std::string, Variant> &variantObj);

    VariantObj(const VariantObj &param);

    VariantObj(VariantObj &&param);

    VariantObj &operator=(const VariantObj &param);

    VariantObj &operator=(VariantObj &&param);

    VariantObj& operator=(const std::unordered_map<std::string, utils::Variant>& variantObj);

    template <typename T, typename std::enable_if<std::is_same<T, std::unordered_map<std::string, Variant>>::value, bool>::type = true>
    operator T() const;

private:
    std::unordered_map<std::string, Variant> mVariantObj;
};

template <typename T, typename std::enable_if<std::is_same<T, std::unordered_map<std::string, Variant>>::value, bool>::type>
inline VariantObj::operator T() const
{
    return mVariantObj;
}

}

#endif