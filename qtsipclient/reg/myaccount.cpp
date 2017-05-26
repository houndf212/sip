#include "myaccount.h"
#include <iostream>

using namespace std;

MyAccount::~MyAccount()
{
    std::cout << "*** Account is being deleted: No of calls="
              << calls.size() << std::endl;
}

void MyAccount::removeCall(Call *call)
{
    for (std::vector<Call *>::iterator it = calls.begin();
         it != calls.end(); ++it)
    {
        if (*it == call) {
            calls.erase(it);
            break;
        }
    }
}

void MyAccount::onRegStarted(OnRegStartedParam &prm)
{
    Q_EMIT sig_RegStarted(prm);
}

void MyAccount::onRegState(OnRegStateParam &prm)
{
    qDebug() << Q_FUNC_INFO;
    qDebug() << QThread::currentThread();
    Q_EMIT sig_RegStatus(prm);
}

void MyAccount::onInstantMessage(OnInstantMessageParam &prm)
{
    Q_EMIT sig_IM(prm);
}

//void MyAccount::onIncomingCall(OnIncomingCallParam &iprm)
//{
//    Call *call = new MyCall(*this, iprm.callId);
//    CallInfo ci = call->getInfo();
//    CallOpParam prm;

//    std::cout << "*** Incoming Call: " <<  ci.remoteUri << " ["
//              << ci.stateText << "]" << std::endl;

//    calls.push_back(call);
//    prm.statusCode = (pjsip_status_code)200;
//    call->answer(prm);
//}
