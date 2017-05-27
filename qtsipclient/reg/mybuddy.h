#ifndef MYBUDDY_H
#define MYBUDDY_H
#include <QtCore>
#include <pjsua2.hpp>
using namespace pj;

class MyBuddy : public QObject, public Buddy
{
    Q_OBJECT
public:
    MyBuddy();
    Q_SIGNAL void sig_State();

    // Buddy interface
public:
    virtual void onBuddyState() override;
};

#endif // MYBUDDY_H
