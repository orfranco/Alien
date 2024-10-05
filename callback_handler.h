#ifndef CALLBACKHANDLER_H
#define CALLBACKHANDLER_H

#include <xsensdot_pc_sdk.h>
#include <xstypes/xstime.h>
#include <xstypes/xsportinfoarray.h>
#include <xscommon/xsens_mutex.h>

#include <map>
#include <list>
#include <string>
#include <QDebug>

class CallbackHandler : public XsDotCallback
{
public:
    CallbackHandler(size_t maxBufferSize = 5);
    virtual ~CallbackHandler() noexcept;

    XsPortInfoArray getDetectedDots() const;
    bool errorReceived() const;
    bool packetsAvailable() const;
    bool packetAvailable(const XsString& bluetoothAddress) const;
    XsDataPacket getNextPacket(const XsString& bluetoothAddress);

protected:
    void onAdvertisementFound(const XsPortInfo* portInfo) override;
    void onBatteryUpdated(XsDotDevice* device, int batteryLevel, int chargingStatus) override;
    void onError(const XsString* error) override;
    void onLiveDataAvailable(XsDotDevice* device, const XsDataPacket* packet) override;

private:
    mutable xsens::Mutex m_mutex;
    bool m_errorReceived = false;
    XsPortInfoArray m_detectedDots;

    size_t m_maxNumberOfPacketsInBuffer;
    std::map<XsString, size_t> m_numberOfPacketsInBuffer;
    std::map<XsString, std::list<XsDataPacket>> m_packetBuffer;
};

#endif // CALLBACKHANDLER_H
