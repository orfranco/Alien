#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDial>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <rtmidi\RtMidi.h>
#include <callback_handler.h>
#include <thread>

class XsDotHandler;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:
    void updateGui(int rollValue, int pitchValue, int yawValue);
    void toggleSendRoll();
    void toggleSendPitch();
    void toggleSendYaw();
    void sendOnlyRoll();
    void sendOnlyPitch();
    void sendOnlyYaw();

private:
    QDial* m_rollDial;
    QDial* m_pitchDial;
    QDial* m_yawDial;

    QLabel* m_rollLabel;
    QLabel* m_pitchLabel;
    QLabel* m_yawLabel;

    QCheckBox* m_sendRollToggle;
    QCheckBox* m_sendPitchToggle;
    QCheckBox* m_sendYawToggle;

    QPushButton* m_sendOnlyRollButton;
    QPushButton* m_sendOnlyPitchButton;
    QPushButton* m_sendOnlyYawButton;

    RtMidiOut midiOut;
    XsDotHandler* m_xsDotHandler;

    bool sendRoll = true;
    bool sendPitch = true;
    bool sendYaw = true;

    void setupMidi();
    void sendMidiMessage(int value, int ccChannel);
};

#endif  // MAINWINDOW_H
