#include "main_window.h"
#include <QDebug>
#include <vector>
#include <callback_handler.h>
#include <thread>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_manager(nullptr)
{
    // Set up the GUI
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    m_dial = new QDial(this);
    m_dial->setRange(0, 127);
    m_dial->setValue(64);
    layout->addWidget(m_dial);

    m_label = new QLabel("MIDI Value: 64", this);
    layout->addWidget(m_label);

    // Set up MIDI
    try {
        if (midiOut.getPortCount() == 0) {
            qDebug() << "No available MIDI output ports.";
            return;
        }
        midiOut.openPort(2);
    }
    catch (RtMidiError& error) {
        qDebug() << "MIDI Error: " << error.what();
    }

    // Initialize Xsens SDK
    m_manager = XsDotConnectionManager::construct();
    if (m_manager == nullptr) {
        qDebug() << "Failed to create XsDotConnectionManager.";
        return;
    }

    m_callbackHandler = new CallbackHandler();
    m_manager->addXsDotCallbackHandler(m_callbackHandler);

    // Start the Xsens connection in a separate thread
    std::thread xsensThread([this]() {
        runXsensConnection();  // Run the connection logic in a separate thread
        });
    xsensThread.detach();  // Detach the thread so it runs independently
}

MainWindow::~MainWindow()
{
    // Cleanup
    if (m_manager) {
        m_manager->destruct();
    }

    delete m_callbackHandler;
}

void MainWindow::runXsensConnection()
{
    // Scan for Xsens DOT devices
    m_manager->enableDeviceDetection();
    qDebug() << "Scanning for Xsens DOT devices...";

    // Wait for a while for devices to be detected
    XsTime::msleep(5000);  // Simulate waiting for device detection (blocking)

    m_manager->disableDeviceDetection();

    if (m_callbackHandler->getDetectedDots().empty()) {
        qDebug() << "No devices found.";
        return;
    }

    qDebug() << "Found " << m_callbackHandler->getDetectedDots().size() << " Dots";

    // Open the detected devices
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
        auto filterProfiles = device->getAvailableFilterProfiles();
        qDebug() << filterProfiles.size() << " available filter profiles:";
        for (auto& f : filterProfiles)
            qDebug() << f.label();

        qDebug() << "Current profile: " << device->onboardFilterProfile().label();
        if (device->setOnboardFilterProfile(XsString("General")))
            qDebug() << "Succesfully set profile to General";
        else
            qDebug() << "Setting filter profile failed!";

        qDebug() << "Setting quaternion CSV output";
        device->setLogOptions(XsLogOptions::Euler);

        qDebug() << "Putting device into measurement mode.";
        if (!device->startMeasurement(XsPayloadMode::ExtendedEuler))
        {
            qDebug() << "Could not put device into measurement mode. Reason: " << m_manager->lastResultText().toStdString().c_str();
            continue;
        }
    }

    while (true) {
        if (m_callbackHandler->packetsAvailable())
        {
            for (auto const& device : m_deviceList)
            {
                // Retrieve a packet
                XsDataPacket packet = m_callbackHandler->getNextPacket(device->portInfo().bluetoothAddress());

                if (packet.containsOrientation())
                {
                    XsEuler euler = packet.orientationEuler();

                    int value = ((euler.roll() + 180) / 360.0) * 127.0;
                    qDebug() << "got Value: " << value;
                    updateGui(value);
                }
            }
        }
    }
}

void MainWindow::updateGui(int midiValue)
{
    // Update the QDial and QLabel based on the MIDI value
    m_dial->setValue(midiValue);
    m_label->setText(QString("MIDI Value: %1").arg(midiValue));

    // Send the MIDI message
    sendMidiMessage(midiValue);
}

void MainWindow::sendMidiMessage(int value)
{
    // Send a MIDI Control Change message
    std::vector<unsigned char> message;
    message.push_back(0xB0);  // Control Change on channel 1
    message.push_back(9);     // Control number (choose appropriate CC number)
    message.push_back(value);  // MIDI value based on sensor data

    midiOut.sendMessage(&message);
}
