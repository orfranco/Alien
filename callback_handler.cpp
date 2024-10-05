#include "callback_handler.h"
#include "user_settings.h"
#include <cassert>

using namespace std;

CallbackHandler::CallbackHandler(size_t maxBufferSize)
    : m_maxNumberOfPacketsInBuffer(maxBufferSize)
{
}

CallbackHandler::~CallbackHandler() noexcept
{
}

XsPortInfoArray CallbackHandler::getDetectedDots() const
{
    xsens::Lock locky(&m_mutex);
    return m_detectedDots;
}

bool CallbackHandler::errorReceived() const
{
    return m_errorReceived;
}

bool CallbackHandler::packetsAvailable() const
{
    for (auto const& device : m_detectedDots)
        if (!packetAvailable(device.bluetoothAddress()))
            return false;
    return true;
}

bool CallbackHandler::packetAvailable(const XsString& bluetoothAddress) const
{
    xsens::Lock locky(&m_mutex);
    if (m_numberOfPacketsInBuffer.find(bluetoothAddress) == m_numberOfPacketsInBuffer.end())
        return false;
    return m_numberOfPacketsInBuffer.at(bluetoothAddress) > 0;
}

XsDataPacket CallbackHandler::getNextPacket(const XsString& bluetoothAddress)
{
    if (!packetAvailable(bluetoothAddress))
        return XsDataPacket();
    xsens::Lock locky(&m_mutex);
    XsDataPacket oldestPacket(m_packetBuffer[bluetoothAddress].front());
    m_packetBuffer[bluetoothAddress].pop_front();
    --m_numberOfPacketsInBuffer[bluetoothAddress];
    return oldestPacket;
}

void CallbackHandler::onAdvertisementFound(const XsPortInfo* portInfo)
{
    qDebug() << "Dot Found: " << portInfo->bluetoothAddress().toStdString().c_str();
    xsens::Lock locky(&m_mutex);
    if (!UserSettings().m_whiteList.size() || UserSettings().m_whiteList.find(portInfo->bluetoothAddress()) != -1)
        m_detectedDots.push_back(*portInfo);
    else
        qDebug() << "Ignoring " << portInfo->bluetoothAddress().toStdString().c_str();
}

void CallbackHandler::onBatteryUpdated(XsDotDevice* device, int batteryLevel, int chargingStatus)
{
    qDebug() << device->deviceTagName().toStdString().c_str() << " BatteryLevel: " << batteryLevel << " Charging status: " << chargingStatus;
}

void CallbackHandler::onError(const XsString* error)
{
    qDebug() << "Error received: " << *error->toStdString().c_str();
    m_errorReceived = true;
}

void CallbackHandler::onLiveDataAvailable(XsDotDevice* device, const XsDataPacket* packet)
{
    xsens::Lock locky(&m_mutex);
    assert(packet != nullptr);
    while (m_numberOfPacketsInBuffer[device->portInfo().bluetoothAddress()] >= m_maxNumberOfPacketsInBuffer)
        (void)getNextPacket(device->portInfo().bluetoothAddress());

    m_packetBuffer[device->portInfo().bluetoothAddress()].push_back(*packet);
    ++m_numberOfPacketsInBuffer[device->portInfo().bluetoothAddress()];
    assert(m_numberOfPacketsInBuffer[device->portInfo().bluetoothAddress()] <= m_maxNumberOfPacketsInBuffer);
}
