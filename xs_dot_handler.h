#ifndef XS_DOT_HANDLER_H
#define XS_DOT_HANDLER_H

#include <QObject>
#include <xsensdot_pc_sdk.h>
#include <xstypes/xstime.h>
#include <callback_handler.h>
#include <vector>

class MainWindow;

class XsDotHandler
{
public:
    XsDotHandler(MainWindow* mainWindow);
    ~XsDotHandler();

    void run();
    void processPackets();

private:
    XsDotConnectionManager* m_manager;
    CallbackHandler* m_callbackHandler;
    std::vector<XsDotDevice*> m_deviceList;
    MainWindow* m_mainWindow; // Reference to MainWindow
};

#endif // XS_DOT_HANDLER_H
