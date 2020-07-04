#include "uiheader.h"

#include <QDebug>
#include <QLabel>
#include <QPainter>
#include <QStaticText>

UIHeader::UIHeader(QWidget *parent) : QWidget(parent)
{
    setObjectName(__CLASS_NAME__.c_str());

    resize(2000,50);
    setContentsMargins(0,0,0,0);

    label = new QLabel(this);
    label->setText("ZETTELKASTEN");

        label->setStyleSheet(".QLabel{color:#FFFFFF; font: 14px \"St Charles\";}");
    label->setAlignment(Qt::AlignVCenter);
    label->setFixedHeight(20);
    label->setContentsMargins(0,0,0,0);
    label->move(55,16);
    label->adjustSize();


}

void UIHeader::setClientModel(ClientModel *model)
{
    this->clientModel = model;
    if(model)
    {
        // Show warning if this is a prerelease version
        //connect(model, SIGNAL(alertsChanged(QString)), this, SLOT(updateAlerts(QString)));
        //updateAlerts(model->getStatusBarWarnings());
    }
}

void UIHeader::setWalletModel(WalletModel *model)
{
    this->walletModel = model;
    if(model && model->getOptionsModel())
    {
        // Keep up to date with wallet
        setBalance(model->getBalance(), model->getUnconfirmedBalance(), model->getImmatureBalance());
        connect(model, SIGNAL(balanceChanged(qint64, qint64, qint64)), this, SLOT(setBalance(qint64, qint64, qint64)));

        connect(model->getOptionsModel(), SIGNAL(displayUnitChanged(int)), this, SLOT(updateDisplayUnit()));
    }

    // update the display unit, to not use the default ("BTC")
    updateDisplayUnit();
}

void UIHeader::updateDisplayUnit()
{
    if(walletModel && walletModel->getOptionsModel())
    {
        if(currentBalance != -1)
            setBalance(currentBalance, currentUnconfirmedBalance, currentImmatureBalance);

    }
}

void UIHeader::setBalance(qint64 balance, qint64 unconfirmedBalance, qint64 immatureBalance)
{
    unit = walletModel->getOptionsModel()->getDisplayUnit();
    currentBalance = balance;
    currentUnconfirmedBalance = unconfirmedBalance;
    currentImmatureBalance = immatureBalance;
    update();
}

void UIHeader::paintEvent(QPaintEvent *e)
{
    QPainter painter;

    painter.begin(this);
    painter.fillRect(0,0,this->width(),50,QColor(0x101010));

    QPixmap pix_logo(":/ui/icons/logo_inverted");

    painter.drawPixmap(15,13,pix_logo);


    painter.fillRect(199,0,1,50,QColor("#000000"));


    int offset_x = this->width()-50;
    for(int i = 4;i>0;i--){
        painter.fillRect(offset_x,0,50,50,QColor("#333333"));
        painter.fillRect(offset_x,0,3,50,QColor("#3b3b3b"));
        offset_x -= 52;
    }



    QFont f;
    int pixelsWide,pixelsHigh;

    f.setFamily("Open Sans");
    f.setBold(true);
    f.setPixelSize(18);
    painter.setFont(f);
    painter.setPen(QColor("#FF7F00"));
    QStaticText stat;
    QStaticText stat2;
    QStaticText stat_ZTTL;

    painter.setPen(QColor("#BF5F00"));
    stat_ZTTL.setText("ZTTL");

    QFontMetrics fm(f);
    int stat_ZTTL_width, stat_ZTTL_height;
    stat_ZTTL_width = fm.width(stat_ZTTL.text());
    stat_ZTTL_height = fm.height();

    //painter.fillRect(200,10,pixelsWide, pixelsHigh,QColor("#FFFFFF"));
    painter.drawStaticText(450-stat_ZTTL_width/2,25-stat_ZTTL_height/2,stat_ZTTL);

    painter.setPen(QColor("#FF7F00"));
    stat2.setText(BitcoinUnits::format(unit, currentBalance));

    QFontMetrics fm2(f);
    pixelsWide = fm2.width(stat2.text());
    pixelsHigh = fm2.height();



    //painter.fillRect(200,10,pixelsWide, pixelsHigh,QColor("#FFFFFF"));
    painter.drawStaticText(450-10-pixelsWide-stat_ZTTL_width/2,25-pixelsHigh/2,stat2);

    f.setFamily("Open Sans");
    f.setBold(true);
    f.setPixelSize(12);
    painter.setFont(f);
    painter.setPen(QColor("#FF7F00"));
    if(currentUnconfirmedBalance == 0){
        painter.setPen(QColor("#7F3F00"));
    }

    stat.setText(BitcoinUnits::format(unit, currentUnconfirmedBalance));
    painter.drawStaticText(450+10+stat_ZTTL_width/2,5,stat);

    painter.setPen(QColor("#FF7F00"));
    if(currentImmatureBalance == 0){
        painter.setPen(QColor("#7F3F00"));
    }
    stat.setText(BitcoinUnits::format(unit, currentImmatureBalance));
    painter.drawStaticText(450+10+stat_ZTTL_width/2,25,stat);

    painter.end();

}
