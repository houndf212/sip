#include "mycall.h"
#include "myaccount.h"
#include <iostream>
using namespace std;

MyCall::MyCall(MyAccount &acc, int call_id)
    : Call(acc, call_id)
{
    myAcc = &acc;
}

void MyCall::onCallState(OnCallStateParam &prm)
{
    PJ_UNUSED_ARG(prm);

    CallInfo ci = getInfo();
    std::cout << "*** Call: " <<  ci.remoteUri << " [" << ci.stateText
              << "]" << std::endl;

    if (ci.state == PJSIP_INV_STATE_DISCONNECTED) {
        myAcc->removeCall(this);
        /* Delete the call */
        delete this;
    }
}
