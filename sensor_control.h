#ifndef SENSOR_CONTROL_H
#define SENSOR_CONTROL_H

#include <QWidget>
#include "control_dial.h"

class SensorControl : public QWidget
{
    Q_OBJECT

public:
    explicit SensorControl(QWidget* parent = nullptr);

signals:
    void sendMidiMessage(int value, int ccChannel);

public slots:
    void updateValues(int rollValue, int pitchValue, int yawValue);

private:
    ControlDial* m_rollDial;
    ControlDial* m_pitchDial;
    ControlDial* m_yawDial;

private slots:
    void sendOnlyRoll();
    void sendOnlyPitch();
    void sendOnlyYaw();
};

#endif // SENSOR_CONTROL_H
