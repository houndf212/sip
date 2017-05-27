#ifndef BUDDYWIDGET_H
#define BUDDYWIDGET_H

#include <QWidget>
#include "myaccount.h"
#include "mybuddy.h"

namespace Ui {
class BuddyWidget;
}

class BuddyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BuddyWidget(QWidget *parent = 0);
    ~BuddyWidget();
    void setBuddy(MyAccount* myacc, MyBuddy* buddy);
private:
    void setUI();
    Q_SLOT void onBtnIM();
    Q_SLOT void onBtnUpdate();
    Q_SLOT void onBtnDel();

    Q_SLOT void onBuddyState();
private:
    Ui::BuddyWidget *ui;
    MyAccount* m_acc;
    MyBuddy* m_buddy;
};

#endif // BUDDYWIDGET_H
