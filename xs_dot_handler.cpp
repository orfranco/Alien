#include "xs_dot_handler.h"
#include "main_window.h"

XsDotHandler::XsDotHandler(MainWindow* parent)
    : QObject(parent), m_manager(nullptr), m_callbackHandler(nullptr)
{
    m_manager = XsDotConnectionManager::construct();
    if (m_manager == nullptr) {
        qDebug() << "Failed to create XsDotConnectionManager.";
        return;
    }

    m_callbackHandler = new CallbackHandler();
    m_manager->addXsDotCallbackHandler(m_callbackHandler);
}

XsDotHandler::~XsDotHandler()
{
    if (m_manager) {
        m_manager->destruct();
    }
    delete m_callbackHandler;
}

void XsDotHandler::run()
{
    m_manager->enableDeviceDetection();
    qDebug() << "Scanning for Xsens DOT devices...";

    XsTime::msleep(10000);  // Wait for device detection

    m_manager->disableDeviceDetection();

    if (m_callbackHandler->getDetectedDots().empty()) {
        qDebug() << "No devices found.";
        return;
    }

    qDebug() << "Found " << m_callbackHandler->getDetectedDots().size() << " Dots";

    initializeDevices();
    processPackets();
}

void XsDotHandler::initializeDevices()
{
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

        qDebug() << "Found a device with tag: " << device->deviceTagName().toStdString().c_str() << " @ address: " << device->portInfo().bluetoothAddress().toStdString().c_str();

        if (!device->setOnboardFilterProfile(XsString("General"))) {
            qDebug() << "Setting filter profile failed!";
        }

        qDebug() << "Setting quaternion CSV output";
        device->setLogOptions(XsLogOptions::Euler);

        if (!device->startMeasurement(XsPayloadMode::ExtendedEuler)) {
            qDebug() << "Could not put device into measurement mode. Reason: " << m_manager->lastResultText().toStdString().c_str();
        }
    }
}

void XsDotHandler::processPackets()
{
    while (true) {
        if (m_callbackHandler->packetsAvailable()) {
            for (auto const& device : m_deviceList) {
                XsDataPacket packet = m_callbackHandler->getNextPacket(device->portInfo().bluetoothAddress());

                if (packet.containsOrientation()) {
                    XsEuler euler = packet.orientationEuler();
                    int value = ((euler.roll() + 180) / 360.0) * 127.0;
                    qDebug() << "Got value: " << value;

                    // Emit signal to update the GUI in the main thread
                    QMetaObject::invokeMethod(parent(), "updateGui", Q_ARG(int, value));
                }
            }
        }
    }
}
