#ifndef XSDOTHANDLER_H
#define XSDOTHANDLER_H

#include <QObject>
#include <QDebug>
#include <vector>
#include <callback_handler.h>
#include <xsensdot_pc_sdk.h>
#include <xstypes/xstime.h>
#include <thread>

class MainWindow;  // Forward declaration of MainWindow

class XsDotHandler : public QObject
{
    Q_OBJECT

public:
    explicit XsDotHandler(MainWindow* parent);
    ~XsDotHandler();

    void run();

private:
    XsDotConnectionManager* m_manager;
    CallbackHandler* m_callbackHandler;
    std::vector<XsDotDevice*> m_deviceList;

    void initializeDevices();
    void processPackets();
};

#endif  // XSDOTHANDLER_H
