#include "imdlg.h"
#include "ui_imdlg.h"

IMDlg::IMDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IMDlg),
    m_myacc(nullptr),
    m_buddy(nullptr)
{
    ui->setupUi(this);
    connect(ui->btnSend, &QPushButton::clicked,
            this, &IMDlg::onBtnSend);

    ui->lblSendStatus->clear();
}

IMDlg::~IMDlg()
{
    delete ui;
    delUuids();
}

void IMDlg::setBuddy(MyAccount *myacc, MyBuddy *buddy)
{
    m_myacc = myacc;
    m_buddy = buddy;
    connect(m_myacc, &MyAccount::sig_IM_Status,
            this, &IMDlg::onSendStatus);
}

void IMDlg::delUuids()
{
    for (QUuid* uuid : msg_map.keys())
    {
        delete uuid;
    }
}

void IMDlg::onBtnSend()
{
    ui->lblSendStatus->clear();
    std::string msg = ui->txtMsg->toPlainText().toStdString();

    SendInstantMessageParam prm;
    prm.contentType = "text/plain";
    prm.content = msg;
    QUuid *uuid = new QUuid(QUuid::createUuid());
    prm.userData = uuid;
    msg_map.insert(uuid, msg);
    m_buddy->sendInstantMessage(prm);
}

void IMDlg::onSendStatus(OnInstantMessageStatusParam prm)
{
    Q_ASSERT(sender() == m_myacc);
    QUuid *uuid = reinterpret_cast<QUuid*>(prm.userData);
    Q_ASSERT(msg_map.contains(uuid));
    msg_map.remove(uuid);
    delete uuid;
    ui->lblSendStatus->setText(QString::fromStdString(prm.reason));
}
