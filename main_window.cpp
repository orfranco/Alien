#include "main_window.h"
#include "xs_dot_handler.h"
#include "sensor_control.h"
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_xsDotHandler(nullptr)
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    setupMidi();

    m_xsDotHandler = new XsDotHandler(this);
    QFuture<std::list<std::string>> future = QtConcurrent::run([this]() {
        return m_xsDotHandler->connectDots();
        });

    QFutureWatcher<std::list<std::string>>* watcher = new QFutureWatcher<std::list<std::string>>(this);
    connect(watcher, &QFutureWatcher<std::list<std::string>>::finished, [this, watcher, mainLayout]() {
        std::list<std::string> connectedDots = watcher->result();

        setupSensorsControls(connectedDots, *mainLayout);

        watcher->deleteLater();

        QThread* xsensThread = new QThread();
        m_xsDotHandler->moveToThread(xsensThread);
        connect(xsensThread, &QThread::started, m_xsDotHandler, &XsDotHandler::processPackets);
        connect(xsensThread, &QThread::finished, xsensThread, &QObject::deleteLater);

        xsensThread->start();
        });

    watcher->setFuture(future);
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

void MainWindow::setupSensorsControls(std::list<std::string> connectedDots, QVBoxLayout& mainLayout)
{
    for (const auto& bluetoothAddress : connectedDots) {
        auto sensorControl = new SensorControl(this);

        mainLayout.addWidget(sensorControl);

        connect(sensorControl, &SensorControl::sendMidiMessage, this, &MainWindow::sendMidiMessage);

        m_sensorsControls[bluetoothAddress] = std::move(sensorControl);
    }
}

void MainWindow::updateGui(std::string bluetoothAddress, int rollValue, int pitchValue, int yawValue)
{
    m_sensorsControls[bluetoothAddress]->updateValues(rollValue, pitchValue, yawValue);
}

void MainWindow::sendMidiMessage(int value, int ccChannel)
{
    std::vector<unsigned char> message;
    message.push_back(0xB0);      // Control Change (CC) message
    message.push_back(ccChannel); // MIDI CC channel
    message.push_back(value);     // MIDI value based on sensor data

    midiOut.sendMessage(&message);
}
