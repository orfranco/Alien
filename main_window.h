#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <rtmidi\RtMidi.h>
#include "sensor_control.h"

class XsDotHandler;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:
    void updateGui(int rollValue, int pitchValue, int yawValue);
    void sendMidiMessage(int value, int ccChannel);

private:
    void setupMidi();

    SensorControl* m_sensorControl;
    XsDotHandler* m_xsDotHandler;
    RtMidiOut midiOut;
};

#endif // MAIN_WINDOW_H