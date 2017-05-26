#include "dialog.h"
#include "ui_dialog.h"
#include <string>
#include <iostream>
using namespace std;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    uiConnect();
    //test
    ui->txtIdUri->setText( "sip:1007@192.168.7.234" );
    ui->txtDomain->setText( "sip:192.168.7.234" );
    ui->txtAcc->setText( "1007" );
    ui->txtPwd->setText("1234");
}

Dialog::~Dialog()
{
    delete ui;
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

void Dialog::onRegStart(OnRegStartedParam prm)
{
    qDebug() << Q_FUNC_INFO <<" : "<<prm.renew;
}

void Dialog::onRegParam(OnRegStateParam prm)
{
    qDebug() << Q_FUNC_INFO;
    qDebug() << QThread::currentThread();
    MyAccount *acc = dynamic_cast<MyAccount *>(sender());
    Q_ASSERT(acc == m_acc.data());
    AccountInfo ai = acc->getInfo();

    std::cout << (ai.regIsActive? "*** Register: code=" : "*** Unregister: code=")
              << prm.code << std::endl;
    if (ai.regIsActive)
    {
        ui->lblReg->setText("Reg");
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
