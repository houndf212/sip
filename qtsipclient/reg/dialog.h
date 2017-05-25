#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <myaccount.h>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
private:
    void uiConnect();
    void accConnect(MyAccount* acc);
    void renew(bool b);
    Q_SLOT void onBtnReg();
    Q_SLOT void onBtnRenew();
    Q_SLOT void onBtnUnreg();
    Q_SLOT void onRegStart(OnRegStartedParam prm);
    Q_SLOT void onRegParam(OnRegStateParam prm);
    Q_SLOT void onIM(OnInstantMessageParam prm);

private:
    Ui::Dialog *ui;
    QScopedPointer<MyAccount> m_acc;
};

#endif // DIALOG_H
