#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDial>
#include <QLabel>
#include <rtmidi/RtMidi.h>
#include <callback_handler.h>
#include <thread>

class XsDotHandler;  // Forward declaration of XsDotHandler

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:
    void updateGui(int midiValue);

private:
    QDial* m_dial;
    QLabel* m_label;
    RtMidiOut midiOut;
    XsDotHandler* m_xsDotHandler;

    void setupMidi();
    void sendMidiMessage(int value);
};

#endif  // MAINWINDOW_H
