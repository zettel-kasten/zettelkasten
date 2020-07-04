#include "uitoolbar.h"

#include <QDebug>

UIToolBar::UIToolBar(QWidget *parent) : QWidget(parent)
{
    resize(2000,50);
    setContentsMargins(0,0,0,0);

    int offset_x = 10;
    buttonSendCoins = new QPushButton(this);
    buttonSendCoins->setCheckable(true);
    buttonSendCoins->setIcon(QIcon(":/icons/send"));
    buttonSendCoins->setText(tr("  &Send"));
    buttonSendCoins->move(offset_x, 10);
    buttonSendCoins->resize(buttonSendCoins->width(),30);

    offset_x += buttonSendCoins->width()+10;

    buttonReceiveCoins = new QPushButton(this);
    buttonReceiveCoins->setCheckable(true);
    buttonReceiveCoins->setText(tr("  &Receive"));
    buttonReceiveCoins->setIcon(QIcon(":/icons/receiving_addresses"));
    buttonReceiveCoins->move(offset_x, 10);
    buttonReceiveCoins->resize(buttonSendCoins->width(),30);

    offset_x += buttonReceiveCoins->width()+10;

    buttonHistoryPage = new QPushButton(this);
    buttonHistoryPage->setCheckable(true);
    buttonHistoryPage->setText(tr("  &Transactions"));
    buttonHistoryPage->setIcon(QIcon(":/icons/history"));
    buttonHistoryPage->move(offset_x, 10);
    buttonHistoryPage->resize(buttonHistoryPage->width(),30);

    offset_x += buttonHistoryPage->width()+10;

    buttonAddressBookPage = new QPushButton(this);
    buttonAddressBookPage->setCheckable(true);
    buttonAddressBookPage->setText(tr("  &Addresses"));
    buttonAddressBookPage->setIcon(QIcon(":/icons/address-book"));
    buttonAddressBookPage->move(offset_x, 10);
    buttonAddressBookPage->resize(buttonAddressBookPage->width(),30);

    offset_x += buttonAddressBookPage->width()+10;

    connect(buttonSendCoins,SIGNAL(released()),this,SLOT(releasedButtonSendCoins()));
    connect(buttonReceiveCoins,SIGNAL(released()),this,SLOT(releasedButtonReceiveCoins()));
    connect(buttonHistoryPage,SIGNAL(released()),this,SLOT(releasedButtonHistoryPage()));
    connect(buttonAddressBookPage,SIGNAL(released()),this,SLOT(releasedButtonAddressBookPage()));

    //Tab index 2:

    offset_x = 10;
    buttonCPUMiner = new QPushButton(this);
    buttonCPUMiner->setCheckable(true);
    buttonCPUMiner->setText(tr("  &CPU Miner"));
    buttonCPUMiner->setIcon(QIcon(":/icons/tx_mined"));
    buttonCPUMiner->move(offset_x, 10);
    buttonCPUMiner->resize(buttonCPUMiner->width(),30);

    offset_x += buttonCPUMiner->width()+10;

    connect(buttonCPUMiner,SIGNAL(released()),this,SLOT(releasedButtonCPUMiner()));

    //

    switchToToolBar(0);
}

void UIToolBar::uncheckButtons()
{
    buttonSendCoins->setChecked(false);
    buttonReceiveCoins->setChecked(false);
    buttonHistoryPage->setChecked(false);
    buttonAddressBookPage->setChecked(false);
    buttonCPUMiner->setChecked(false);
}

void UIToolBar::switchToToolBar(int index)
{
    buttonSendCoins->setVisible(false);
    buttonReceiveCoins->setVisible(false);
    buttonHistoryPage->setVisible(false);
    buttonAddressBookPage->setVisible(false);
    buttonCPUMiner->setVisible(false);
    if(index == 0){

    }
    if(index == 1){
        buttonSendCoins->setVisible(true);
        buttonReceiveCoins->setVisible(true);
        buttonHistoryPage->setVisible(true);
        buttonAddressBookPage->setVisible(true);
    }

    if(index == 2){
        buttonCPUMiner->setVisible(true);
    }

    uncheckButtons();
    if(clicked_button == 0){
        buttonSendCoins->setChecked(true);
    }
    if(clicked_button == 1){
        buttonReceiveCoins->setChecked(true);
    }
    if(clicked_button == 2){
        buttonHistoryPage->setChecked(true);
    }
    if(clicked_button == 3){
        buttonAddressBookPage->setChecked(true);
    }
    if(clicked_button == 4){
        buttonCPUMiner->setChecked(true);
    }
}

void UIToolBar::releasedButtonSendCoins()
{
    clicked_button = 0;
    emit signalButtonSendCoins();
}

void UIToolBar::releasedButtonReceiveCoins()
{
    clicked_button = 1;
    emit signalButtonReceiveCoins();
}

void UIToolBar::releasedButtonHistoryPage()
{
    clicked_button = 2;
    emit signalButtonHistoryPage();
}

void UIToolBar::releasedButtonAddressBookPage()
{
    clicked_button = 3;
    emit signalButtonAddressBookPage();
}

void UIToolBar::releasedButtonCPUMiner()
{
    clicked_button = 4;
    emit signalButtonCPUMiner();
}

void UIToolBar::paintEvent(QPaintEvent *e)
{

    painter.begin(this);
    //painter.fillRect(0,0,200,this->height(),QColor(0x333333));
    painter.fillRect(0,0,this->width(),50,QColor(0xDFDFDF));

    //painter.fillRect(0,50,this->width(),2,QColor(0x3b3b3b));
    painter.fillRect(0,50,this->width(),3,QColor(0xcfcfcf));
    painter.end();
}
