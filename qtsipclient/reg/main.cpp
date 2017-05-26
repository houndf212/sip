#include <QApplication>
#include <pjsua2.hpp>
#include <iostream>
#include <QtCore>

#include "dialog.h"

using namespace std;
using namespace pj;

void initEPconfig(bool bThread)
{
    // Init library
    EpConfig ep_cfg;
    if (!bThread)
    {
        //in QCoreApplication::exec()
        //To make your application perform idle processing (by executing a special function whenever there are no pending events),
        //use a QTimer with 0 timeout.

        ep_cfg.uaConfig.threadCnt = 0;
        ep_cfg.uaConfig.mainThreadOnly = true;
        QTimer *timer = new QTimer(qApp);
        // I do not know, so 100 is OK?
        const int k_interval = 100;
        QObject::connect(timer, &QTimer::timeout,
                         [](){Endpoint::instance().libHandleEvents(k_interval);});
        timer->start(0);
    }
//    ep_cfg.uaConfig.stunServer.push_back("192.168.7.234");
//    ep_cfg.uaConfig.mwiUnsolicitedEnabled = false;
    ep_cfg.uaConfig.userAgent = "pjsip with Qt";
    ep_cfg.logConfig.level = 5;
    Endpoint::instance().libInit( ep_cfg );
}

void initTransport()
{
    // Transport
    {
        TransportConfig tcfg;
//        tcfg.port = 5060;
        TransportId udpid = Endpoint::instance().transportCreate(PJSIP_TRANSPORT_UDP, tcfg);
        Q_ASSERT(udpid>=0);
    }

     //Transport
//    {
//        TransportConfig tcfg;
////        tcfg.port = 5060;
//        TransportId tcpid = Endpoint::instance().transportCreate(PJSIP_TRANSPORT_TCP, tcfg);
//        Q_ASSERT(tcpid>=0);
//    }
}

void regQtParam()
{
    qRegisterMetaType<OnRegStateParam>("OnRegStateParam");
    qRegisterMetaType<OnRegStartedParam>("OnRegStartedParam");
    qRegisterMetaType<OnInstantMessageParam>("OnInstantMessageParam");
}

int main(int argc, char *argv[])
{
    regQtParam();
    QApplication a(argc, argv);
//Buddy
//UaConfig
    Endpoint ep;
    try
    {
        ep.libCreate();
        initEPconfig(true);
        initTransport();
        ep.libStart();
        // use Qt quit to call libdestory
        // or do nothing ~EndPoint() will do also
        QObject::connect(&a, &QCoreApplication::aboutToQuit,
                         [](){Endpoint::instance().libDestroy();});
    }
    catch (Error & err)
    {
        std::cout << "Exception: " << err.info() << std::endl;
        return 1;
    }

    Dialog w;
    w.show();
    qDebug() << "GUI thread: " << QThread::currentThread();
    int ret = a.exec();
    qDebug() << "#### return main!";
    return ret;
}
