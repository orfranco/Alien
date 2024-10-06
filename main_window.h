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


class CallbackHandler;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void updateGui(int midiValue);
    void runXsensConnection();

private:
    QDial* m_dial;
    QLabel* m_label;

    RtMidiOut midiOut;

    XsDotConnectionManager* m_manager;
    XsDotDevicePtrArray m_deviceList;

    CallbackHandler* m_callbackHandler;

    void sendMidiMessage(int value);
};

#endif // MAINWINDOW_H