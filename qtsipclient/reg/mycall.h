#ifndef MYCALL_H
#define MYCALL_H
#include <pjsua2.hpp>

using namespace pj;
class MyAccount;

class MyCall : public Call
{
private:
    MyAccount *myAcc;
public:
    MyCall(MyAccount &acc, int call_id = PJSUA_INVALID_ID);

    virtual void onCallState(OnCallStateParam &prm);
};

#endif // MYCALL_H
