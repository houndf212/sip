#include "buddywidget.h"
#include "ui_buddywidget.h"
#include "imdlg.h"

BuddyWidget::BuddyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BuddyWidget),
    m_acc(nullptr),
    m_buddy(nullptr)
{
    ui->setupUi(this);

    connect(ui->btnSendIM, &QPushButton::clicked,
            this, &BuddyWidget::onBtnIM);

    connect(ui->btnUpdate, &QPushButton::clicked,
            this, &BuddyWidget::onBtnUpdate);

    connect(ui->btnClear, &QPushButton::clicked,
            this, &BuddyWidget::onBtnDel);
}

BuddyWidget::~BuddyWidget()
{
    delete ui;
}

void BuddyWidget::setBuddy(MyAccount *myacc, MyBuddy *buddy)
{
    m_acc = myacc;
    m_buddy = buddy;
    // buddy 析构函数会自动从acc中删除
    m_buddy->setParent(this);
    connect(m_buddy, &MyBuddy::sig_State,
            this, &BuddyWidget::onBuddyState);
}

void BuddyWidget::setUI()
{
    BuddyInfo bi = m_buddy->getInfo();
    QString uri = QString::fromStdString(bi.uri);
    QString status = QString::fromStdString(bi.presStatus.note);
    ui->lblUri->setText(uri);
    ui->lblStatus->setText(status);
}

void BuddyWidget::onBtnIM()
{
    IMDlg im(this);
    im.setBuddy(m_acc, m_buddy);
    im.exec();
}

void BuddyWidget::onBtnUpdate()
{
    m_buddy->updatePresence();
}

void BuddyWidget::onBtnDel()
{

    this->deleteLater();
}

void BuddyWidget::onBuddyState()
{
    setUI();
}
