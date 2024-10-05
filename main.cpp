#include <QApplication>
#include "main_window.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);  // Initialize Qt

    // Create the main window, which will process Xsens data and send MIDI
    MainWindow mainWindow;
    mainWindow.show();  // Show the GUI

    return app.exec();  // Start the Qt event loop
}