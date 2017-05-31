#include "dialog.h"
#include "ui_dialog.h"
#include <string>
#include <iostream>
#include "buddywidget.h"
using namespace std;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    initOnlineCombo();
    uiConnect();
    initBuddyArea();
    //test
    ui->txtIdUri->setText( "sip:1007@192.168.4.176" );
    ui->txtDomain->setText( "sip:192.168.4.176" );
    ui->txtAcc->setText( "1007" );
    ui->txtPwd->setText("1234");

    ui->txtBuddy->setText("sip:1001@192.168.4.176");

    resize(600, 400);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::initOnlineCombo()
{
    ui->cmbOnlineStatus->addItem("online");
    ui->cmbOnlineStatus->addItem("offline");
    ui->cmbOnlineStatus->addItem("away");
    ui->cmbOnlineStatus->addItem("busy");

    typedef void (QComboBox::*MF)(const QString&);
    MF mf = static_cast<MF>(&QComboBox::activated);
    connect(ui->cmbOnlineStatus, mf,
            this, &Dialog::onOnlieStatusChanged);
}

void Dialog::uiConnect()
{
    connect(ui->btnReg , &QPushButton::clicked,
            this, &Dialog::onBtnReg);

    connect(ui->btnRenew , &QPushButton::clicked,
            this, &Dialog::onBtnRenew);

    connect(ui->btnUnreg , &QPushButton::clicked,
            this, &Dialog::onBtnUnreg);

    connect(ui->btnCleanIM, &QPushButton::clicked,
            ui->txtIM, &QPlainTextEdit::clear);

    connect(ui->btnAddBuddy, &QPushButton::clicked,
            this, &Dialog::onBtnAddBuddy);
}

void Dialog::accConnect(MyAccount *acc)
{
    connect(acc, &MyAccount::sig_RegStatus,
            this, &Dialog::onRegParam);

    connect(acc, &MyAccount::sig_RegStarted,
            this, &Dialog::onRegStart);

    connect(acc, &MyAccount::sig_IM,
            this, &Dialog::onIM);
}

void Dialog::initBuddyArea()
{
    QLayout* layout = new QVBoxLayout;
    ui->buddyArea->widget()->setLayout(layout);
}

void Dialog::renew(bool b)
{
    try
    {
        m_acc->setRegistration(b);
    }
    catch(pj::Error& e)
    {
        cout << e.reason <<endl;
    }
}

void Dialog::onBtnReg()
{
    AccountConfig acc_cfg;
    acc_cfg.idUri = ui->txtIdUri->text().toStdString();
    acc_cfg.regConfig.registrarUri = ui->txtDomain->text().toStdString();
    string accstr = ui->txtAcc->text().toStdString();
    string pwd = ui->txtPwd->text().toStdString();

    acc_cfg.sipConfig.authCreds.push_back( AuthCredInfo("digest", "*",
                                                        accstr, 0, pwd) );
    // TODO 不知道怎么添加别名 难道在 callconfig里面？
//    acc_cfg.sipConfig.contactForced = "centos";

    // pres
    acc_cfg.presConfig.publishEnabled = true;

    MyAccount *acc = new MyAccount;
    accConnect(acc);

    m_acc.reset(acc);
    try
    {
        acc->create(acc_cfg, true);
    } catch(Error& err)
    {
        cout << "Account creation error: " << err.info() << endl;
    }
}

void Dialog::onBtnRenew()
{
    if (m_acc.isNull())
        return;

    renew(true);
}

void Dialog::onBtnUnreg()
{
    if (m_acc.isNull())
        return;
    if (m_acc->getInfo().regIsActive)
        renew(false);
}

void Dialog::onBtnAddBuddy()
{
    if (m_acc.isNull())
        return;

    string buddyuri = ui->txtBuddy->text().toStdString();
    BuddyConfig bc;
    bc.uri = buddyuri;
    MyBuddy* buddy = new MyBuddy;
    buddy->create(*m_acc.data(), bc);

    BuddyWidget* bw = new BuddyWidget;
    bw->setBuddy(m_acc.data(), buddy);
    ui->buddyArea->widget()->layout()->addWidget(bw);
    buddy->subscribePresence(true);

}

void Dialog::onRegStart(OnRegStartedParam prm)
{
    qDebug() << Q_FUNC_INFO <<" "<<prm.renew;
}

void Dialog::onRegParam(OnRegStateParam prm)
{
    qDebug() << Q_FUNC_INFO;
    MyAccount *acc = dynamic_cast<MyAccount *>(sender());
    Q_ASSERT(acc == m_acc.data());
    AccountInfo ai = acc->getInfo();

    std::cout << (ai.regIsActive? "*** Register: code=" : "*** Unregister: code=")
              << prm.code << std::endl;
    if (ai.regIsActive)
    {
        ui->lblReg->setText("Reg");
        QString myuri = QString::fromStdString(ai.uri);
        this->setWindowTitle(myuri);
        ui->cmbOnlineStatus->setCurrentText("online");
    }
    else
    {
        ui->lblReg->setText("Unreg");
        m_acc.reset();
    }
}

void Dialog::onIM(OnInstantMessageParam prm)
{
    if (prm.contentType == "text/plain")
    {
        QString time = QDateTime::currentDateTime().toString("hh:mm");
        QString acc = QString::fromStdString(prm.fromUri);
        QString msg = QString::fromStdString(prm.msgBody);
        ui->txtIM->appendPlainText(time +":\n"+acc+" : "+msg);
    }
}

void Dialog::onOnlieStatusChanged(const QString &txt)
{
    qDebug() << Q_FUNC_INFO<<" "<<txt;
    if (m_acc.isNull())
        return;
    AccountInfo ai = m_acc->getInfo();
    if (!ai.regIsActive)
        return;

    PresenceStatus ps;

    if (txt == "offline")
    {
        ps.status = PJSUA_BUDDY_STATUS_OFFLINE;
        ps.statusText = txt.toStdString();
    }
    else if(txt == "online")
    {
        ps.status = PJSUA_BUDDY_STATUS_ONLINE;
        ps.statusText = txt.toStdString();
    }
    else if(txt == "away")
    {
        ps.status = PJSUA_BUDDY_STATUS_ONLINE;
        ps.activity = PJRPID_ACTIVITY_AWAY;
        ps.note = txt.toStdString();
    }
    else if(txt == "busy")
    {
        ps.status = PJSUA_BUDDY_STATUS_ONLINE;
        ps.activity = PJRPID_ACTIVITY_BUSY;
        ps.note = txt.toStdString();
    }
    else
    {
        Q_ASSERT(false);
    }
    m_acc->setOnlineStatus(ps);
}
