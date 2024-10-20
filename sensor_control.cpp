#include "sensor_control.h"
#include <QHBoxLayout>

SensorControl::SensorControl(QWidget* parent)
    : QWidget(parent)
{
    QHBoxLayout* layout = new QHBoxLayout(this);

    m_rollDial = new ControlDial("Roll", this);
    m_pitchDial = new ControlDial("Pitch", this);
    m_yawDial = new ControlDial("Yaw", this);

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
    turnOffAllSensorsControls();
    m_rollDial->toggleSendStateWithParam(true);
}

void SensorControl::sendOnlyPitch()
{
    turnOffAllSensorsControls();
    m_pitchDial->toggleSendStateWithParam(true);
}

void SensorControl::sendOnlyYaw()
{
    turnOffAllSensorsControls();
    m_yawDial->toggleSendStateWithParam(true);
}

void SensorControl::turnOff()
{
    m_rollDial->toggleSendStateWithParam(false);
    m_pitchDial->toggleSendStateWithParam(false);
    m_yawDial->toggleSendStateWithParam(false);
}
