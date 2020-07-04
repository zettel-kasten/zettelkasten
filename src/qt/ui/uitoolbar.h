#ifndef UITOOLBAR_H
#define UITOOLBAR_H

#include "uihelper.h"

#include <QPushButton>
#include <QToolButton>

class UIToolBar : public QWidget
{
    Q_OBJECT
public:
    explicit UIToolBar(QWidget *parent = nullptr);

    //Tab index 1
    QPushButton *buttonSendCoins;
    QPushButton *buttonReceiveCoins;
    QPushButton *buttonHistoryPage;
    QPushButton *buttonAddressBookPage;

    //Tab index 2
    QPushButton *buttonCPUMiner;

    int clicked_button;
    void uncheckButtons();
signals:
    void signalButtonSendCoins();
    void signalButtonReceiveCoins();
    void signalButtonHistoryPage();
    void signalButtonAddressBookPage();
    void signalButtonCPUMiner();
public slots:

    void switchToToolBar(int index);

    void releasedButtonSendCoins();
    void releasedButtonReceiveCoins();
    void releasedButtonHistoryPage();
    void releasedButtonAddressBookPage();
    void releasedButtonCPUMiner();
protected:
    QPainter painter;
    void paintEvent(QPaintEvent *e);
};

#endif // UITOOLBAR_H
