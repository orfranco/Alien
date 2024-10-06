#include "main_window.h"
#include "xs_dot_handler.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_xsDotHandler(nullptr)
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    QHBoxLayout* rollLayout = new QHBoxLayout();
    m_rollDial = new QDial(this);
    m_rollDial->setRange(0, 127);
    m_rollDial->setValue(64);
    m_rollLabel = new QLabel("Roll MIDI Value: 64", this);
    m_sendRollToggle = new QCheckBox("Send Roll", this);
    m_sendOnlyRollButton = new QPushButton("Send Only Roll", this);
    rollLayout->addWidget(m_rollDial);
    rollLayout->addWidget(m_rollLabel);
    rollLayout->addWidget(m_sendRollToggle);
    rollLayout->addWidget(m_sendOnlyRollButton);
    mainLayout->addLayout(rollLayout);

    m_sendRollToggle->setStyleSheet(
        "QCheckBox::indicator { width: 50px; height: 25px; }"
        "QCheckBox::indicator:checked { background-color: red; border-radius: 12px; }"
        "QCheckBox::indicator:unchecked { background-color: green; border-radius: 12px; }"
    );
    connect(m_sendRollToggle, &QCheckBox::stateChanged, this, &MainWindow::toggleSendRoll);
    connect(m_sendOnlyRollButton, &QPushButton::clicked, this, &MainWindow::sendOnlyRoll);

    QHBoxLayout* pitchLayout = new QHBoxLayout();
    m_pitchDial = new QDial(this);
    m_pitchDial->setRange(0, 127);
    m_pitchDial->setValue(64);
    m_pitchLabel = new QLabel("Pitch MIDI Value: 64", this);
    m_sendPitchToggle = new QCheckBox("Send Pitch", this);
    m_sendOnlyPitchButton = new QPushButton("Send Only Pitch", this);
    pitchLayout->addWidget(m_pitchDial);
    pitchLayout->addWidget(m_pitchLabel);
    pitchLayout->addWidget(m_sendPitchToggle);
    pitchLayout->addWidget(m_sendOnlyPitchButton);
    mainLayout->addLayout(pitchLayout);

    m_sendPitchToggle->setStyleSheet(
        "QCheckBox::indicator { width: 50px; height: 25px; }"
        "QCheckBox::indicator:checked { background-color: red; border-radius: 12px; }"
        "QCheckBox::indicator:unchecked { background-color: green; border-radius: 12px; }"
    );
    connect(m_sendPitchToggle, &QCheckBox::stateChanged, this, &MainWindow::toggleSendPitch);
    connect(m_sendOnlyPitchButton, &QPushButton::clicked, this, &MainWindow::sendOnlyPitch);

    QHBoxLayout* yawLayout = new QHBoxLayout();
    m_yawDial = new QDial(this);
    m_yawDial->setRange(0, 127);
    m_yawDial->setValue(64);
    m_yawLabel = new QLabel("Yaw MIDI Value: 64", this);
    m_sendYawToggle = new QCheckBox("Send Yaw", this);
    m_sendOnlyYawButton = new QPushButton("Send Only Yaw", this);
    yawLayout->addWidget(m_yawDial);
    yawLayout->addWidget(m_yawLabel);
    yawLayout->addWidget(m_sendYawToggle);
    yawLayout->addWidget(m_sendOnlyYawButton);
    mainLayout->addLayout(yawLayout);

    m_sendYawToggle->setStyleSheet(
        "QCheckBox::indicator { width: 50px; height: 25px; }"
        "QCheckBox::indicator:checked { background-color: red; border-radius: 12px; }"
        "QCheckBox::indicator:unchecked { background-color: green; border-radius: 12px; }"
    );
    connect(m_sendYawToggle, &QCheckBox::stateChanged, this, &MainWindow::toggleSendYaw);
    connect(m_sendOnlyYawButton, &QPushButton::clicked, this, &MainWindow::sendOnlyYaw);

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

void MainWindow::updateGui(int rollValue, int pitchValue, int yawValue)
{
    m_rollDial->setValue(rollValue);
    m_rollLabel->setText(QString("Roll MIDI Value: %1").arg(rollValue));

    m_pitchDial->setValue(pitchValue);
    m_pitchLabel->setText(QString("Pitch MIDI Value: %1").arg(pitchValue));

    m_yawDial->setValue(yawValue);
    m_yawLabel->setText(QString("Yaw MIDI Value: %1").arg(yawValue));

    if (!m_sendRollToggle->isChecked()) sendMidiMessage(rollValue, 9);     // Roll on CC 9
    if (!m_sendPitchToggle->isChecked()) sendMidiMessage(pitchValue, 10);  // Pitch on CC 10
    if (!m_sendYawToggle->isChecked()) sendMidiMessage(yawValue, 11);      // Yaw on CC 11
}

void MainWindow::sendMidiMessage(int value, int ccChannel)
{
    std::vector<unsigned char> message;
    message.push_back(0xB0);      // Control Change (CC) message
    message.push_back(ccChannel); // MIDI CC channel
    message.push_back(value);     // MIDI value based on sensor data

    midiOut.sendMessage(&message);
}

void MainWindow::toggleSendRoll()
{
    sendRoll = !m_sendRollToggle->isChecked();
}

void MainWindow::toggleSendPitch()
{
    sendPitch = !m_sendPitchToggle->isChecked();
}

void MainWindow::toggleSendYaw()
{
    sendYaw = !m_sendYawToggle->isChecked();
}

void MainWindow::sendOnlyRoll()
{
    sendRoll = true;
    sendPitch = false;
    sendYaw = false;
    m_sendRollToggle->setChecked(false);
    m_sendPitchToggle->setChecked(true);
    m_sendYawToggle->setChecked(true);
}

void MainWindow::sendOnlyPitch()
{
    sendRoll = false;
    sendPitch = true;
    sendYaw = false;
    m_sendRollToggle->setChecked(true);
    m_sendPitchToggle->setChecked(false);
    m_sendYawToggle->setChecked(true);
}

void MainWindow::sendOnlyYaw()
{
    sendRoll = false;
    sendPitch = false;
    sendYaw = true;
    m_sendRollToggle->setChecked(true);
    m_sendPitchToggle->setChecked(true);
    m_sendYawToggle->setChecked(false);
}
