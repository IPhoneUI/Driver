#ifndef STORAGES_REPOSITORY_H
#define STORAGES_REPOSITORY_H

#include <string>
#include <shared_mutex>
#include <utils/filesystem.h>
#include <utils/Variant.h>
#include <unordered_map>

namespace common {

namespace ParameterEnumeration {
enum Type {
    SIM_PhoneNumber,
    SIM_Network,
    SIM_PhoneSignal,
    SIM_WifiPassword,
    SIM_AllowAccess,
    SIM_CellularStatus,
    SIM_MaxCompatibility,
    SIM_Contact,
    SIM_History,
    FMem_Recording,
    FMem_DeleteRecording,
    FMem_AirPlaneMode,
    Wifi_Data,
    Speaker_Muted,
    Speaker_Volume,
    ParameterMax

};
}
typedef ParameterEnumeration::Type ParameterIndex;

struct ConfigParameter {
    ConfigParameter()
    {}

    ConfigParameter(const std::string& keyVal, ParameterIndex indexVal)
    {
        name = keyVal;
        index = indexVal;
    }

    bool operator!=(const ConfigParameter& other) const 
    {
        return !(*this == other);
    }

    bool operator==(const ConfigParameter& other) const 
    {
        return name == other.name;
    }

    std::string name;
    ParameterIndex index;
    utils::Variant value;
};

class Repository {
public:
    Repository()
    {}
    
    Repository(const std::string& name);

    std::string name() const
    {
        return mName;
    }

    bool pull();

    utils::Variant operator[](ParameterIndex index);

    void addParameter(const std::string& keyName, ParameterIndex index);

private:
    ConfigParameter* findParameter(const std::string& name);

    std::string mName;
    std::string mPath;

    mutable std::shared_mutex mMutex;
    std::vector<ConfigParameter*> mConfigParameters;
};

}

#endif