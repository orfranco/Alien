#include "main_window.h"
#include "xs_dot_handler.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_xsDotHandler(nullptr)
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* layout = new QVBoxLayout(centralWidget);
    m_dial = new QDial(this);
    m_dial->setRange(0, 127);
    m_dial->setValue(64);
    layout->addWidget(m_dial);
    m_label = new QLabel("MIDI Value: 64", this);
    layout->addWidget(m_label);

    setupMidi();

    m_xsDotHandler = new XsDotHandler(this);
    std::thread xsensThread([this]() {
        m_xsDotHandler->run();
        });
    xsensThread.detach();
}

MainWindow::~MainWindow()
{
    delete m_xsDotHandler;
}

void MainWindow::setupMidi()
{
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
}

void MainWindow::updateGui(int midiValue)
{
    m_dial->setValue(midiValue);
    m_label->setText(QString("MIDI Value: %1").arg(midiValue));
    sendMidiMessage(midiValue);
}

void MainWindow::sendMidiMessage(int value)
{
    std::vector<unsigned char> message;
    message.push_back(0xB0);  // Control Change (CC) Message
    message.push_back(9);     // Control number
    message.push_back(value);  // MIDI value based on sensor data

    midiOut.sendMessage(&message);
}
