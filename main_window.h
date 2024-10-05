#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDial>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <xsensdot_pc_sdk.h>
#include <xstypes/xstime.h>
#include <xstypes/xsportinfoarray.h>
#include <xscommon/xsens_mutex.h>

#include <map>
#include <list>
#include <string>

#include "conio.h"
#include "user_settings.h"
#include <iostream>
#include <vector>
#include <string>
#include <rtmidi/RtMidi.h>


class CallbackHandler;  // Forward declaration for the callback handler

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void updateGui(int midiValue);  // Slot to update GUI based on the callback
    void runXsensConnection();

private:
    QDial* m_dial;
    QLabel* m_label;

    RtMidiOut midiOut;  // MIDI output object

    XsDotConnectionManager* m_manager;
    XsDotDevicePtrArray m_deviceList;

    CallbackHandler* m_callbackHandler;  // Callback handler instance

    void sendMidiMessage(int value);  // Method to send MIDI messages
};

#endif // MAINWINDOW_H