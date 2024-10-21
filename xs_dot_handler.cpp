#include "xs_dot_handler.h"
#include "main_window.h"
#include <QDebug>

XsDotHandler::XsDotHandler(MainWindow* mainWindow)
    : m_mainWindow(mainWindow), m_manager(XsDotConnectionManager::construct()), m_callbackHandler(new CallbackHandler())
{
    if (!m_manager) {
        qDebug() << "Failed to create XsDotConnectionManager.";
        return;
    }

    m_manager->addXsDotCallbackHandler(m_callbackHandler);
}

XsDotHandler::~XsDotHandler()
{
    if (m_manager) {
        m_manager->destruct();
    }

    delete m_callbackHandler;
}

std::list<std::string> XsDotHandler::connectDots()
{
    std::list<std::string> bluetoothAddresses;

    m_manager->enableDeviceDetection();
    qDebug() << "Scanning for Xsens DOT devices...";

    XsTime::msleep(10000);

    m_manager->disableDeviceDetection();

    if (m_callbackHandler->getDetectedDots().empty()) {
        qDebug() << "No devices found.";
        return bluetoothAddresses;
    }

    qDebug() << "Found " << m_callbackHandler->getDetectedDots().size() << " Dots";

    for (auto& portInfo : m_callbackHandler->getDetectedDots()) {
        qDebug() << "Connecting to device: " << portInfo.bluetoothAddress().toStdString().c_str();

        if (!m_manager->openPort(portInfo)) {
            qDebug() << "Connection to Device " << portInfo.bluetoothAddress().toStdString().c_str() << " failed, retrying...";
            if (!m_manager->openPort(portInfo)) {
                qDebug() << "Could not open DOT. Reason: " << m_manager->lastResultText().toStdString().c_str();
                continue;
            }
        }

        XsDotDevice* device = m_manager->device(portInfo.deviceId());
        if (device == nullptr) {
            qDebug() << "Failed to initialize device.";
            continue;
        }

        m_deviceList.push_back(device);
        bluetoothAddresses.push_back(portInfo.bluetoothAddress().toStdString());

        device->setOutputRate(60);
        qDebug() << "OutputRate: " << device->outputRate();

    }

    return bluetoothAddresses;
}

void XsDotHandler::processPackets()
{
    for (auto const& device : m_deviceList) {
        qDebug() << "Starting Measurments for device: " << device->portInfo().bluetoothAddress().toStdString().c_str();
        if (!device->startMeasurement(XsPayloadMode::OrientationEuler)) {
            qDebug() << "Could not put device into measurement mode. Reason: " << m_manager->lastResultText().toStdString().c_str();
            continue;
        }
    }

    bool orientationResetDone = false;
    int64_t lastOrientationReset = XsTime::timeStampNow();
    while (true) {
        for (auto const& device : m_deviceList) {
            if (m_callbackHandler->packetAvailable(device->portInfo().bluetoothAddress())) {
                XsDataPacket packet = m_callbackHandler->getNextPacket(device->portInfo().bluetoothAddress());

                if (packet.containsOrientation()) {
                    XsEuler euler = packet.orientationEuler();

                    int rollValue = ((euler.roll() + 180.0) / 360.0) * 128.0;
                    int pitchValue = ((euler.pitch() + 90) / 180) * 128.0; // Pitch values are between -90 to 90 as opposed to roll & yaw (-180 to 180)
                    int yawValue = ((euler.yaw() + 180.0) / 360.0) * 128.0;

                    m_mainWindow->updateGui(device->portInfo().bluetoothAddress().toStdString(), rollValue, pitchValue, yawValue);
                }
            }
        }

        if ((XsTime::timeStampNow() - lastOrientationReset) > 50000)
        {
            for (auto const& device : m_deviceList)
            {
                qDebug() << "Resetting heading for device " << device->portInfo().bluetoothAddress().toStdString().c_str() << ":";
                if (device->resetOrientation(XRM_Heading) && device->resetOrientation(XRM_Object))
                    qDebug() << "OK";
                else
                    qDebug() << "NOK: " << device->lastResultText().toStdString().c_str();
            }
            lastOrientationReset = XsTime::timeStampNow();
        }
    }
}
