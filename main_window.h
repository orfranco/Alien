#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDial>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <vector>
#include <rtmidi/RtMidi.h>

class XsDotHandler;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void updateGui(int rollValue, int pitchValue, int yawValue);

private:
    RtMidiOut midiOut;

    XsDotHandler* m_xsDotHandler;

    // Roll components
    QDial* m_rollDial;
    QLabel* m_rollLabel;
    QCheckBox* m_sendRollToggle;
    QPushButton* m_sendOnlyRollButton;
    bool sendRoll = true;

    // Pitch components
    QDial* m_pitchDial;
    QLabel* m_pitchLabel;
    QCheckBox* m_sendPitchToggle;
    QPushButton* m_sendOnlyPitchButton;
    bool sendPitch = true;

    // Yaw components
    QDial* m_yawDial;
    QLabel* m_yawLabel;
    QCheckBox* m_sendYawToggle;
    QPushButton* m_sendOnlyYawButton;
    bool sendYaw = true; 

    void setupMidi();
    void setToggleStyles();

    void sendMidiMessage(int value, int ccChannel);

private slots:
    void toggleSendRoll();
    void toggleSendPitch();
    void toggleSendYaw();
    void sendOnlyRoll();
    void sendOnlyPitch();
    void sendOnlyYaw();
};

#endif // MAINWINDOW_H
