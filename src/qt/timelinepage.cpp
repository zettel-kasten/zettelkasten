#include "timelinepage.h"
#include "ui_timelinepage.h"

#include "main.h"
#include "bitcoinrpc.h"
#include "util.h"

#include <QDateTime>
#include <QLabel>
#include <QImage>
#include <QPixmap>
#include <QStaticText>

#include <QtMath>

#include <QDebug>

TimelinePage::TimelinePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimelinePage),
    show_preloader(true),
    scroll_in(0),
    ui_block_height(1000000),
    ui_block_height_previous(999999)
{
    ui->setupUi(this);

    QDateTime cd = QDateTime::currentDateTime();
    qsrand(cd.toTime_t());

    tx_set.empty();

    timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(33);

    timerData = new QTimer(this);
    //timer->setTimerType(Qt::PreciseTimer);
    connect(timerData, SIGNAL(timeout()), this, SLOT(updateData()));
    timerData->start(500);

    m_time.start();
}

TimelinePage::~TimelinePage()
{
    delete ui;
}

void TimelinePage::setClientModel(ClientModel *model)
{
    this->clientModel = model;
    if(model)
    {
        // Show warning if this is a prerelease version
        //connect(model, SIGNAL(alertsChanged(QString)), this, SLOT(updateAlerts(QString)));
        //updateAlerts(model->getStatusBarWarnings());
    }
}

void TimelinePage::setWalletModel(WalletModel *model)
{
    this->walletModel = model;
    if(model && model->getOptionsModel())
    {
        // Set up transaction list
        /*filter = new TransactionFilterProxy();
        filter->setSourceModel(model->getTransactionTableModel());
        filter->setLimit(NUM_ITEMS);
        filter->setDynamicSortFilter(true);
        filter->setSortRole(Qt::EditRole);
        filter->sort(TransactionTableModel::Status, Qt::DescendingOrder);

        ui->listTransactions->setModel(filter);
        ui->listTransactions->setModelColumn(TransactionTableModel::ToAddress);*/

        // Keep up to date with wallet
        //setBalance(model->getBalance(), model->getUnconfirmedBalance(), model->getImmatureBalance());
        //connect(model, SIGNAL(balanceChanged(qint64, qint64, qint64)), this, SLOT(setBalance(qint64, qint64, qint64)));

        //connect(model->getOptionsModel(), SIGNAL(displayUnitChanged(int)), this, SLOT(updateDisplayUnit()));
    }

    // update the display unit, to not use the default ("BTC")
    updateDisplayUnit();
}

void TimelinePage::updateDisplayUnit()
{
    if(walletModel && walletModel->getOptionsModel())
    {


    }
}

void TimelinePage::paintCircle(int x, int y, int radius, int inner_radius, QString color_str)
{
    p.setPen(Qt::NoPen);

    brush.setColor("#FFFFFF");
    brush.setStyle(Qt::SolidPattern);
    p.setBrush(brush);
    p.drawEllipse(x-radius,y-radius,radius*2,radius*2);

    brush.setColor(color_str);
    brush.setStyle(Qt::SolidPattern);
    p.setBrush(brush);
    p.drawEllipse(x-inner_radius,y-inner_radius,inner_radius*2,inner_radius*2);
}

void TimelinePage::paintBubble(int x, int y, int width, int height, QString text, int orientation, QString _wordsalad)
{

    QFont f;
    f.setFamily("Open Sans");
    f.setBold(true);
    f.setPixelSize(16);
    p.setFont(f);

    int stat_width, stat_height;
    QFontMetrics fm(f);

    QStaticText stat;

    stat.setText(text);

    stat_width = fm.width(stat.text());
    stat_height = fm.height();

    p.setPen(Qt::NoPen);

    brush.setColor("#FFFFFF");
    brush.setStyle(Qt::SolidPattern);
    p.setBrush(brush);

    QPainterPath path;
    path.moveTo(x+25*orientation,y);
    path.lineTo(x+40*orientation,y+8);
    path.lineTo(x+40*orientation,y-8);

    p.fillPath(path, brush);
    if(orientation == 1){
        p.drawRoundedRect(x+40,y-height/2,stat_width+40,height,10,10);
    } else{
        p.drawRoundedRect(x-40-stat_width-40,y-height/2,stat_width+40,height,10,10);
    }

    p.setPen(QColor("#000000"));

    if(orientation == 1){
        p.drawStaticText(x+60,y-stat_height/2,stat);
    } else{
        p.drawStaticText(x-60-stat_width,y-stat_height/2,stat);
    }

    p.setOpacity(1);
}

void TimelinePage::drawRotatedCenteredImage(int x, int y, qreal angle, QString img_source){
    QImage srcImg(img_source);
    QMatrix matrix;
    matrix.rotate(angle);
    QImage dstImg = srcImg.transformed(matrix);
    p.drawImage(x-dstImg.width()/2,y-dstImg.height()/2,srcImg.transformed(matrix));
}

void TimelinePage::updateData()
{
    if(isVisible() && !show_preloader){
        CBlockIndex* pindex = pindexBest;

        int height = nBestHeight;

        std::vector<uint256> vtxid;
        mempool.queryHashes(vtxid);

        //Creation

        QString tx_id;
        for(int i = 0;i<vtxid.size();i++){
            tx_id = vtxid.at(i).GetHex().c_str();
            if(!tx_set.contains(tx_id)){
                tx_set.insert(tx_id);

                /*struct ui_tx_item {
                    QString txid;
                    QString txid_truncated;
                    int offset_x;
                    int offset_y;
                    int target_x;
                    int target_y;
                    qreal counter;
                    int sin_sign;
                    qreal sin_divider;
                    qreal sin_multiplier;
                    int cos_sign;
                    qreal cos_divider;
                    qreal cos_multiplier;
                    int state;
                    qreal opacity;
                    int win_orientation;
                };*/

                qreal temp_counter = qrand()%100000;
                int temp_sin_sign = qPow(-1,qrand()%10);
                qreal temp_sin_divider = 10+qrand()%20;
                qreal temp_sin_multiplier = 200+qrand()%100;
                int temp_cos_sign = qPow(-1,qrand()%10);
                qreal temp_cos_divider = 10+qrand()%20;
                qreal temp_cos_multiplier = 45+qrand()%30;

                ui_tx_list.append({tx_id,
                                   QString(tx_id).left(10)+QString(" . . ."),
                                   0,-50, //offset
                                   0,75, //target offset
                                   temp_counter, //counter
                                   temp_sin_sign,
                                   temp_sin_divider,
                                   temp_sin_multiplier, //sin_multiplier
                                   temp_cos_sign,
                                   temp_cos_divider,
                                   temp_cos_multiplier, // cos_multiplier
                                   0,
                                   1, //opacity
                                   1 //win orientation
                                  });
            }
        }

        //Deletion
        if(vtxid.size()<tx_set.size()){
            foreach (const QString &value__, tx_set){
                uint256 item;
                const char *cstr = value__.toStdString().c_str();
                item.SetHex(cstr);
                if ( std::find(vtxid.begin(), vtxid.end(), item) != vtxid.end() ){
                } else{
                    //NOT found in vector, means you can delete it";
                    for(int i2 = 0;i2<ui_tx_list.length();i2++){
                        if(ui_tx_list[i2].txid == value__){
                            ui_tx_list[i2].state = 1;
                            ui_tx_list[i2].opacity = 1;
                            ((height)%2 ==1) ? ui_tx_list[i2].win_orientation = 1:ui_tx_list[i2].win_orientation = -1;
                        }
                    }
                    tx_set.remove(value__);
                }
            }
        }

        ui_block_height = height;

        //one time event to update paint
        if(ui_block_height!=ui_block_height_previous){
            scroll_in = -70;
        }

        ui_block_height_previous = ui_block_height;
    }
}

void TimelinePage::passSyncData(QStringList str_list, int _count, int _nTotalBlocks, int _secs)
{
    show_preloader = !(_secs < 90*60 && _count >= _nTotalBlocks);
}

void TimelinePage::paintEvent(QPaintEvent *e)
{

    int middle_x = this->width()/2;

    p.begin(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.setRenderHint(QPainter::HighQualityAntialiasing);
    //p.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    p.fillRect(0,0,this->width(),this->height(),QColor(0xeff0f4));

    p.fillRect(0,0,3,this->height(),QColor(0xcfcfcf));

    QPen pen;

    pen.setColor("#dfdfdf");
    pen.setStyle(Qt::DotLine);
    pen.setWidth(6);
    p.setPen(pen);

    p.drawLine(middle_x,0,middle_x,200);

    p.fillRect(middle_x-3,200,6,this->height(),QColor("#d6dadd"));


    if(show_preloader){
        static qreal starter2 = 0;
        starter2 = (float(m_time.elapsed()) / 1000.0f)*20;
        drawRotatedCenteredImage(this->width()/2, this->height()/2, starter2*20,":/ui/icons/preloader");
    } else{

        int orientation = 1;

        if(scroll_in <0){
            scroll_in *=0.85;
        }

        static int starter = 0;

        for(int i = 0;i<=30;i++){

            p.setOpacity(1);
            if(i == 0){
                p.setOpacity(1-1*(scroll_in/-70));
            }
            paintCircle(middle_x,starter+200+i*70+scroll_in,10,6,"#FF7F00");
            ((ui_block_height-i)%2 ==1) ? orientation = 1:orientation = -1;
            paintBubble(middle_x,starter+200+i*70+scroll_in,160,40,"#"+QString::number(ui_block_height-i),orientation,"love cheese burger");
            orientation*=-1;
        }

        //Transactions

        QFont f;
        f.setFamily("Open Sans");
        f.setBold(false);
        f.setPixelSize(12);
        p.setFont(f);
        p.setPen(QColor("#000000"));

        QStaticText stat;
        stat.setPerformanceHint(QStaticText::AggressiveCaching);

        qreal frame_time = qreal(m_time.elapsed()) / qreal(1000)*5;

        //movement:
        qreal frame_step = (frame_time-previous_frame_time);
        for(int i = 0;i <ui_tx_list.length();i++){
            ui_tx_list[i].counter += frame_step;

            if(ui_tx_list[i].state == 0){
                ui_tx_list[i].offset_x = middle_x;
                if(ui_tx_list[i].offset_y<75 ){
                    ui_tx_list[i].offset_y +=3;
                }
            }

            if(ui_tx_list[i].state == 1){
                ui_tx_list[i].opacity -= 0.03;
                int t = middle_x+ui_tx_list[i].win_orientation*100;
                if(ui_tx_list[i].offset_x <= t){
                    ui_tx_list[i].offset_x+= (t-ui_tx_list[i].offset_x)/30;
                };
                if(ui_tx_list[i].offset_x > t){
                    ui_tx_list[i].offset_x-= (ui_tx_list[i].offset_x-t)/30;
                };
                ui_tx_list[i].sin_multiplier *= 0.9;
                ui_tx_list[i].cos_multiplier *= 0.9;
                if(ui_tx_list[i].offset_y<270 ){
                    ui_tx_list[i].offset_y += (270-ui_tx_list[i].offset_y)/30;
                }
            }
        }

        int temp_x, temp_y, stat_width, stat_height;
        QFontMetrics fm(f);

        for(int i = 0;i <ui_tx_list.length();i++){
            stat.setText(ui_tx_list.at(i).txid_truncated);
            stat_width = fm.width(stat.text());
            stat_height = fm.height();
            temp_x = ui_tx_list.at(i).offset_x
                    +ui_tx_list.at(i).sin_sign
                    *qSin(ui_tx_list.at(i).counter/ui_tx_list.at(i).sin_divider)
                    *ui_tx_list.at(i).sin_multiplier*this->width()/700;
            temp_y = ui_tx_list.at(i).offset_y
                    +ui_tx_list.at(i).cos_sign
                    *qCos(ui_tx_list.at(i).counter/ui_tx_list.at(i).cos_divider)
                    *ui_tx_list.at(i).cos_multiplier;

            p.setOpacity(ui_tx_list[i].opacity);

            p.fillRect(temp_x-stat_width/2-2,
                       temp_y-stat_height/2-2,
                       stat_width+4,
                       stat_height+4,
                       QColor(0xFFAF3F));
            p.drawStaticText(temp_x-stat_width/2,
                             temp_y-stat_height/2,
                             stat);
        }

        //for next frame
        previous_frame_time = frame_time;
    }

    //Garbage-Collection

    for(int i = ui_tx_list.length()-1;i >=0;i--){
        if(ui_tx_list[i].state == 1){
            if(ui_tx_list[i].opacity <= 0){
                qDebug() << "garbage collect" << i;
                ui_tx_list.removeAt(i);
            }
        }
    }

    p.end();
}

