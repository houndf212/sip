#include "mybuddy.h"

MyBuddy::MyBuddy()
{

}

void MyBuddy::onBuddyState()
{
    qDebug() << Q_FUNC_INFO;
    Q_EMIT sig_State();
}
