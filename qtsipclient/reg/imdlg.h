#ifndef IMDLG_H
#define IMDLG_H

#include <QDialog>
#include "myaccount.h"
#include "mybuddy.h"

namespace Ui {
class IMDlg;
}

class IMDlg : public QDialog
{
    Q_OBJECT

public:
    explicit IMDlg(QWidget *parent = 0);
    ~IMDlg();
    void setBuddy(MyAccount* myacc, MyBuddy* buddy);
private:
    void delUuids();
    Q_SLOT void onBtnSend();
    Q_SLOT void onSendStatus(OnInstantMessageStatusParam prm);
private:
    Ui::IMDlg *ui;
    MyAccount* m_myacc;
    MyBuddy* m_buddy;
    QMap<QUuid*, std::string> msg_map;

};

#endif // IMDLG_H
