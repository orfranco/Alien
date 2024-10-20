#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <rtmidi\RtMidi.h>
#include "sensor_control.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QThread>
#include <QObject>

class XsDotHandler;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:
    void updateGui(std::string bluetoothAddress, int rollValue, int pitchValue, int yawValue);
    void sendMidiMessage(int value, int ccChannel);

private:
    void setupMidi();
    void setupSensorsControls(std::list<std::string> connectedDots, QVBoxLayout& mainLayout);

    SensorControl* m_sensorControl;
    std::unordered_map<std::string, SensorControl*> m_sensorsControls;
    XsDotHandler* m_xsDotHandler;
    RtMidiOut midiOut;
};

#endif // MAIN_WINDOW_H