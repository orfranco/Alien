#include "main_window.h"
#include "xs_dot_handler.h"
#include "sensor_control.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QThread>
#include <QObject>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_xsDotHandler(nullptr)
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    // Create SensorControl for Roll, Pitch, and Yaw
    m_sensorControl = new SensorControl(this);
    mainLayout->addWidget(m_sensorControl);

    connect(m_sensorControl, &SensorControl::sendMidiMessage, this, &MainWindow::sendMidiMessage);

    setupMidi();

    m_xsDotHandler = new XsDotHandler(this);
    QThread* xsensThread = new QThread();
    m_xsDotHandler->moveToThread(xsensThread);
    connect(xsensThread, &QThread::started, m_xsDotHandler, &XsDotHandler::run);
    connect(xsensThread, &QThread::finished, xsensThread, &QObject::deleteLater);

    xsensThread->start();
}

MainWindow::~MainWindow()
{
    delete m_xsDotHandler;
}

void MainWindow::setupMidi()
{
    unsigned int nPorts = midiOut.getPortCount();
    std::string targetPortName = "Alien";
    int targetPortIndex = -1;

    for (unsigned int i = 0; i < nPorts; i++) {
        if (midiOut.getPortName(i).find(targetPortName) != std::string::npos) {
            targetPortIndex = i;
            break;
        }
    }

    if (targetPortIndex == -1) {
        qDebug() << "MIDI output port 'Alien' not found.";
        throw std::runtime_error("MIDI output port 'Alien' not found.");
    }

    midiOut.openPort(targetPortIndex);
    qDebug() << "Opened port 'Alien' at index" << targetPortIndex << ".";
}

void MainWindow::updateGui(int rollValue, int pitchValue, int yawValue)
{
    m_sensorControl->updateValues(rollValue, pitchValue, yawValue);
}

void MainWindow::sendMidiMessage(int value, int ccChannel)
{
    std::vector<unsigned char> message;
    message.push_back(0xB0);      // Control Change (CC) message
    message.push_back(ccChannel); // MIDI CC channel
    message.push_back(value);     // MIDI value based on sensor data

    midiOut.sendMessage(&message);
}
