#include "uifooter.h"

#include <QDebug>
#include <QLabel>
#include <QPainter>
#include <QStaticText>
#include "clientmodel.h"
#include "walletmodel.h"
#include "bitcoinunits.h"
#include "optionsmodel.h"
#include "transactiontablemodel.h"
#include "transactionfilterproxy.h"
#include "guiutil.h"
#include "guiconstants.h"
#include "main.h"
#ifndef Q_MOC_RUN
#include "main.h"
#include "bitcoinrpc.h"
#include "util.h"
#endif

extern json_spirit::Value GetNetworkHashPS(int lookup, int height);
extern int64 GetTotalSupply();

static QString formatHashrate(int64 n)
{
    if (n == 0)
        return "0 H/s";

    int i = (int)floor(log(n)/log(1000));
    float v = n*pow(1000.0f, -i);

    QString prefix = "";
    if (i >= 1 && i < 9)
        prefix = " kMGTPEZY"[i];

    return QString("%1 %2H/s").arg(v, 0, 'f', 2).arg(prefix);
}

UIFooter::UIFooter(QWidget *parent) : QWidget(parent)
{
    setObjectName(__CLASS_NAME__.c_str());
    show_preloader = true;

    resize(2000,75);
    setContentsMargins(0,0,0,0);

    numConnections = 0;

    qDebug() << "uifooter constructor";

    QList<QString> label_text;
    label_text.append("Block Count:");
    label_text.append("-");
    label_text.append("Total Coinsupply:");
    label_text.append("-");
    label_text.append("Network Hashrate:");
    label_text.append("-");
    label_text.append("Difficulty:");
    label_text.append("-");

    int offset_x = 10;

    for(int i = 0; i<label_text.count();i=i+2){
        label.append(new QLabel(this));
        label[i]->setText(label_text[i]);
        label[i]->setStyleSheet(".QLabel{color:#EEEEEE; font: 11px \"Open Sans\";}");
        label[i]->setAlignment(Qt::AlignVCenter);
        label[i]->setFixedHeight(13);
        label[i]->setContentsMargins(0,0,0,0);
        label[i]->move(offset_x,5);
        label[i]->adjustSize();

        offset_x += label[i]->sizeHint().width()+10;

        label.append(new QLabel(this));
        label[i+1]->setText(label_text[i+1]);
        label[i+1]->setStyleSheet(".QLabel{color:#EE6F00; font: 11px \"Open Sans\";}");
        label[i+1]->setAlignment(Qt::AlignVCenter);
        label[i+1]->setFixedHeight(13);
        label[i+1]->setContentsMargins(0,0,0,0);
        label[i+1]->move(offset_x,5);
        label[i+1]->adjustSize();

        offset_x += label[i+1]->sizeHint().width()+10;
        offset_x += 30;
    }

}
void UIFooter::updateDataLabels()
{
    CBlockIndex* pindex = pindexBest;
    CBlockIndex* itr = pindex;

    if(itr != NULL){

        label[1]->setText(QString::number(nBestHeight));

        int totalMoney = (double)GetTotalSupply()/(double)COIN;
        label[3]->setText(QString::number( totalMoney)+" ZTTL");

        int64 NetworkHashrate = GetNetworkHashPS(120, -1).get_int64();
        label[5]->setText(formatHashrate(NetworkHashrate));

        label[7]->setText(QString::number(GetDifficulty(itr)));

    }

    int offset_x = 10;
    for(int i = 0; i<8;i=i+2){
        label[i]->move(offset_x,5);
        label[i]->adjustSize();

        offset_x += label[i]->sizeHint().width()+10;

        label[i+1]->move(offset_x,5);
        label[i+1]->adjustSize();

        offset_x += label[i+1]->sizeHint().width()+10;
        offset_x += 30;
    }
}

void UIFooter::setNumConnections(int num)
{
    numConnections = num;
    update();
}

void UIFooter::setProgressInfo(QStringList str_list, int _count, int _nTotalBlocks, int _secs, BlockSource _blockSource )
{
    count = _count;
    nTotalBlocks = _nTotalBlocks;
    if(nTotalBlocks == 0){
        nTotalBlocks = 100000000;
    }
    progress_str_list = str_list;
    secs = _secs;

    blockSource = _blockSource;

    show_preloader = !(_secs < 90*60 && _count >= _nTotalBlocks);

    //Show preloader as empty if no block source
    if(blockSource == BLOCK_SOURCE_NONE){
        count = 0;
    }

    update();
}
void UIFooter::paintEvent(QPaintEvent *e)
{
    QPainter painter;

    painter.begin(this);
    painter.fillRect(0,0,this->width(),75,QColor("#101010"));
    painter.fillRect(0,25,this->width(),3,QColor("#3B3B3B"));

    if(show_preloader)
    {

        painter.fillRect(200,55,this->width()-200-120,10,QColor(0x4F4F4F));
        painter.fillRect(200,55,(this->width()-200-120)*(qreal)count/(qreal)nTotalBlocks,10,QColor(0xFF7F00));

        int stat_width, stat_height;
        QFont f;
        f.setFamily("Open Sans");
        f.setBold(false);
        f.setPixelSize(11);
        painter.setFont(f);
        painter.setPen(QColor("#FF7F00"));
        QStaticText stat;
        stat.setText(progress_str_list.at(0));
        painter.drawStaticText(200,35,stat);

        stat.setText(progress_str_list.at(1));
        QFontMetrics fm(f);

        stat_width = fm.width(stat.text());
        stat_height = fm.height();
        painter.drawStaticText(200+(this->width()-200-120)/2-stat_width/2,35,stat);

        if(blockSource != BLOCK_SOURCE_NONE){
            QString str = QString("%1 total blocks (estimated)").arg(nTotalBlocks);
            QStaticText stat3;
            stat3.setText(str);
            stat_width = fm.width(str);
            stat_height = fm.height();
            painter.drawStaticText(200+(this->width()-200-120)-stat_width,35,stat3);
        }
    }

    QString cipher_0 = QString::number(numConnections%10);
    QString cipher_1 = QString::number((numConnections/10)%10);
    QString cipher_2 = QString::number(numConnections/100);

    if(numConnections<100){cipher_2 = "X";};
    if(numConnections<10){cipher_1 = "X";};
    if(numConnections<1){cipher_0 = "X";};

    QPixmap pix_0(":/ui/icons/LED_"+cipher_0);
    QPixmap pix_1(":/ui/icons/LED_"+cipher_1);
    QPixmap pix_2(":/ui/icons/LED_"+cipher_2);

    painter.drawPixmap(this->width()-30,25+10,pix_0);
    painter.drawPixmap(this->width()-30-21,25+10,pix_1);
    painter.drawPixmap(this->width()-30-42,25+10,pix_2);

    painter.end();
}
