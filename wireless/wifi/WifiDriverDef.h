#ifndef WIFIDRIVERDEF_H
#define WIFIDRIVERDEF_H

/**
 * @enum WifiSpeedMode
 * @brief define speed mode for wifi device
*/
enum class WifiSpeedMode {
    Weak,
    Medium,
    Strong
};

struct WifiDiscoveryDeviceInfo {
    WifiDiscoveryDeviceInfo(const std::string& newName, const std::string& newAddress, const bool& newPrivateAddress, const WifiSpeedMode& newSpeedMode)
    {
        name = newName;
        address = newAddress;
        privateAddr = newPrivateAddress;
        speedMode = newSpeedMode;
    }

    WifiDiscoveryDeviceInfo()
    {
    }

    std::string name {""};
    std::string address {""};
    bool privateAddr {true};
    WifiSpeedMode speedMode {WifiSpeedMode::Medium};
};

struct WifiDeviceInfo
{
    WifiDeviceInfo(const WifiDiscoveryDeviceInfo& newInfo, const std::string& newPassword, const bool& newAutoConnectStatus)
    {
        deviceInfo = newInfo;
        password = newPassword;
        autoConnectStatus = newAutoConnectStatus;
    }

    WifiDeviceInfo() 
    {    
    }

    WifiDiscoveryDeviceInfo deviceInfo;
    std::string password {""};
    bool autoConnectStatus {true};
};

#endif