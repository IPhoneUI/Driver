#include "WifiPairing.h"

#define SERVICE_NAME highlightString("WifiParing").c_str()

namespace driver {

WifiPairing::WifiPairing()
{
    mWifiProvider = WifiProvider::instance();
    mWifiDeploy = WifiDeploy::instance();
}

WifiPairing::~WifiPairing()
{
}

WifiPairing& WifiPairing::instance()
{
    static WifiPairing ins;
    return ins;
}

void WifiPairing::execute(milliseconds delta)
{
    // if (mPairingFlag)
    // {
    //     mTime += delta;
    //     if (mStep == 0)
    //     {
    //         mAuthenStatus = WifiAuthenDeviceStatus::Authencating;
    //         mEvent.push(WifiEvent::AuthenStatus);
    //         mTime = milliseconds(0);
    //         mStep++;
    //     }
    //     if (mStep == 1 && mTime > milliseconds(2000))
    //     {
    //         /** CASE: Authen Fail 
    //          *  mAuthenStatus = WifiAuthenDeviceStatus::Fail;
    //             mEvent.push(WifiEvent::AuthenStatus);
    //             mTime = milliseconds(0);
    //             mStep = 0;
    //             mPairingFlag = false;
    //          **/ 
            

    //         mAuthenStatus = WifiAuthenDeviceStatus::AuthenSuccess;
    //         mPairingDevice = peripherals::WifiDriver::instance().getPairedDevice(mPairingAddr);
    //         mEvent.push(WifiEvent::AuthenStatus);
    //         mTime = milliseconds(0);
    //         mStep++;
    //     }
    //     if (mStep == 2 && mTime > milliseconds(1000))
    //     {
    //         if (peripherals::WifiDriver::instance().removePairedDevice(mPairingAddr))
    //         {
    //             mEvent.push(WifiEvent::PairedDeviceChange);
    //         }
    //         mTime = milliseconds(0);
    //         mStep++;
    //     }
    //     if (mStep == 3 && mTime > milliseconds(100))
    //     {
    //         if (peripherals::WifiDriver::instance().addPairedDevice(peripherals::WifiDriver::instance().getConnectedDevice()))
    //         {
    //             mEvent.push(WifiEvent::PairedDeviceChange);
    //         }
    //         peripherals::WifiDriver::instance().setConnectedDevice(mPairingDevice);
    //         mEvent.push(WifiEvent::UpdateConnectedDevice);
    //         mTime = milliseconds(0);
    //         mStep = 0;
    //         mPairingFlag = false;
    //     }
        
    // }

    // if (!mEvent.isEmpty())
    // {
    //     WifiEvent event = mEvent.front();
    //     switch (event)
    //     {
    //     case WifiEvent::AuthenStatus:
    //         mServiceEvent->onAuthenStatusChanged.emit(mPairingAddr, mAuthenStatus);
    //         break;
    //     case WifiEvent::PairedDeviceChange:
    //         mServiceEvent->onPairedDeviceListChanged.emit();
    //         break;
    //     case WifiEvent::UpdateConnectedDevice: {
    //         auto device = peripherals::WifiDriver::instance().getConnectedDevice();
    //         mServiceEvent->onConnectedDeviceChanged.emit(device);
    //         break;
    //     }
    //     default:
    //         LOG_WARN("Invalid Callback Event");
    //         break;
    //     }

    //     mEvent.pop();
    // }
}

std::list<WifiDeviceShmem> WifiPairing::getPairedDeviceList()
{
    return mWifiProvider->getPairedDeviceList();
}

void WifiPairing::connectDevice(const std::string& addr)
{
    if (mPairingFlag) {
        mStep = 0;
        mTime = milliseconds(0);
    }

    mPairingFlag = true;
    mPairingAddr = addr;
}

void WifiPairing::resetConnectDeviceFlag()
{
    mStep = 0;
    mTime = milliseconds(0);
    mPairingFlag = false;
}

}