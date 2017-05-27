#ifndef MYACCOUNT_H
#define MYACCOUNT_H
#include <QtCore>
#include <pjsua2.hpp>
#include "mycall.h"

using namespace pj;

class MyAccount : public QObject, public Account
{
    Q_OBJECT
public:
    std::vector<Call *> calls;
public:
    MyAccount() = default;

    ~MyAccount();
    void removeCall(Call *call);
public:
    Q_SIGNAL void sig_RegStarted(OnRegStartedParam prm);
    Q_SIGNAL void sig_RegStatus(OnRegStateParam prm);
    Q_SIGNAL void sig_IM(OnInstantMessageParam prm);
    Q_SIGNAL void sig_IM_Status(OnInstantMessageStatusParam prm);
    // Account interface
public:
//    virtual void onIncomingCall(OnIncomingCallParam &prm) override;
    virtual void onRegStarted(OnRegStartedParam &prm) override;
    virtual void onRegState(OnRegStateParam &prm) override;
    virtual void onIncomingSubscribe(OnIncomingSubscribeParam &prm) override;
    virtual void onInstantMessage(OnInstantMessageParam &prm) override;
    virtual void onInstantMessageStatus(OnInstantMessageStatusParam &prm) override;
//    virtual void onTypingIndication(OnTypingIndicationParam &prm) override;
//    virtual void onMwiInfo(OnMwiInfoParam &prm) override;
};

#endif // MYACCOUNT_H
