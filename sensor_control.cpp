#include "sensor_control.h"
#include <QHBoxLayout>

SensorControl::SensorControl(QWidget* parent)
    : QWidget(parent)
{
    QHBoxLayout* layout = new QHBoxLayout(this);

    m_rollDial = new ControlDial("Roll", 9, this);
    m_pitchDial = new ControlDial("Pitch", 10, this);
    m_yawDial = new ControlDial("Yaw", 11, this);

    layout->addWidget(m_rollDial);
    layout->addWidget(m_pitchDial);
    layout->addWidget(m_yawDial);

    connect(m_rollDial, &ControlDial::sendMidiMessage, this, &SensorControl::sendMidiMessage);
    connect(m_pitchDial, &ControlDial::sendMidiMessage, this, &SensorControl::sendMidiMessage);
    connect(m_yawDial, &ControlDial::sendMidiMessage, this, &SensorControl::sendMidiMessage);

    connect(m_rollDial->getSendOnlyButton(), &QPushButton::clicked, this, &SensorControl::sendOnlyRoll);
    connect(m_pitchDial->getSendOnlyButton(), &QPushButton::clicked, this, &SensorControl::sendOnlyPitch);
    connect(m_yawDial->getSendOnlyButton(), &QPushButton::clicked, this, &SensorControl::sendOnlyYaw);
}

void SensorControl::updateValues(int rollValue, int pitchValue, int yawValue)
{
    m_rollDial->setDialValue(rollValue);
    m_pitchDial->setDialValue(pitchValue);
    m_yawDial->setDialValue(yawValue);
}

void SensorControl::sendOnlyRoll()
{
    m_rollDial->toggleSendStateWithParam(true);
    m_pitchDial->toggleSendStateWithParam(false);
    m_yawDial->toggleSendStateWithParam(false);
}

void SensorControl::sendOnlyPitch()
{
    m_rollDial->toggleSendStateWithParam(false);
    m_pitchDial->toggleSendStateWithParam(true);
    m_yawDial->toggleSendStateWithParam(false);
}

void SensorControl::sendOnlyYaw()
{
    m_rollDial->toggleSendStateWithParam(false);
    m_pitchDial->toggleSendStateWithParam(false);
    m_yawDial->toggleSendStateWithParam(true);
}
