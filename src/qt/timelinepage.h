#ifndef TIMELINEPAGE_H
#define TIMELINEPAGE_H

#include <QWidget>
#include <walletmodel.h>
#include <optionsmodel.h>
#include <clientmodel.h>
#include <QTimer>
#include <QTime>
#include <QPainter>
#include <QSet>
#include <QSetIterator>
#include "bitcoinunits.h"

namespace Ui {
class TimelinePage;
}

class TimelinePage : public QWidget
{
    Q_OBJECT

public:
    explicit TimelinePage(QWidget *parent = 0);
    ~TimelinePage();

    QTimer *timer;
    QTimer *timerData;
    QTime m_time;
    qreal previous_frame_time;
    QPainter p;
    QBrush brush;

    void setClientModel(ClientModel *clientModel);
    void setWalletModel(WalletModel *walletModel);
    void updateDisplayUnit();

    /** Pass sync data on */
    void passSyncData(QStringList str_list, int _count, int _nTotalBlocks, int _secs);

    bool show_preloader;

    void paintBubble(int x, int y, int width, int height, QString text, int orientation, QString _wordsalad);

    void drawRotatedCenteredImage(int x, int y, qreal angle, QString img_source);

    void paintCircle(int x, int y, int radius, int inner_radius, QString color_str);

    qreal scroll_in;
    int ui_block_height, ui_block_height_previous;

    struct ui_tx_item {
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
    };
    QList<ui_tx_item> ui_tx_list;
    QSet<QString> tx_set;

public slots:
    void updateData();
private:
    Ui::TimelinePage *ui;
    ClientModel *clientModel;
    WalletModel *walletModel;
    qint64 currentBalance;
    qint64 currentUnconfirmedBalance;
    qint64 currentImmatureBalance;

    void paintEvent(QPaintEvent *e);
};

#endif // TIMELINEPAGE_H
