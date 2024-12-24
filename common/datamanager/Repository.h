#ifndef STORAGES_REPOSITORY_H
#define STORAGES_REPOSITORY_H

#include <string>
#include <shared_mutex>
#include <utils/filesystem.h>
#include "Parameter.h"
#include <unordered_map>
#include <Connection.h>

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
    Wifi_Status,
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
    Parameter value;
};

class Repository {
public:
    enum State {
        IdleState,
        WaitToPullCompleted,
        PullCompleted,
        PullError,
        WaitToPushCompleted,
        PushCompleted,
        PushError
    };

    Repository() = default;

    std::string name() const
    {
        return mName;
    }

    State state() const
    {
        return mState;
    }

    void setName(const std::string& name);

    void pull();

    void push();

    void setState(State value);

    Parameter& operator[](ParameterIndex index);

    void addParam(const std::string& keyName, ParameterIndex index);

    Signal<State> onRepoStateChanged;

private:
    ConfigParameter* findParam(const std::string& name);

    std::string mName;
    std::string mPath;
    State mState {IdleState};

    mutable std::shared_mutex mMutex;
    std::vector<ConfigParameter*> mConfigParameters;
};

}

#endif