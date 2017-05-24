#include <QApplication>
#include <pjsua2.hpp>
#include <iostream>
#include <QtCore>

#include "dialog.h"

using namespace std;
using namespace pj;

void initEPconfig()
{
    // Init library
    EpConfig ep_cfg;
    ep_cfg.logConfig.level = 3;
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

    // Transport
//    {
//        TransportConfig tcfg;
////        tcfg.port = 5060;
//        TransportId tcpid = Endpoint.instance().transportCreate(PJSIP_TRANSPORT_TCP, tcfg);
//    Q_ASSERT(tcpid>=0);
//    }
}

void regQtParam()
{
    qRegisterMetaType<OnRegStateParam>("OnRegStateParam");
    qRegisterMetaType<OnRegStartedParam>("OnRegStartedParam");
//    qRegisterMetaType<OnInstantMessageParam>("OnInstantMessageParam");
}

int main(int argc, char *argv[])
{
    Endpoint ep;
    try
    {
        ep.libCreate();
        initEPconfig();
        initTransport();
        ep.libStart();
    }
    catch (Error & err)
    {
        std::cout << "Exception: " << err.info() << std::endl;
        return 1;
    }

    regQtParam();
    QApplication a(argc, argv);

    Dialog w;
    w.show();
    qDebug() << "GUI thread: " << QThread::currentThread();
    return a.exec();
}
