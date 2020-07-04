#ifndef UIHEADER_H
#define UIHEADER_H

#include "uihelper.h"

#include <QLabel>
#include <QWidget>
#include <walletmodel.h>
#include <optionsmodel.h>
#include <clientmodel.h>
#include "bitcoinunits.h"

class UIHeader : public QWidget
{
    Q_OBJECT
public:
    explicit UIHeader(QWidget *parent = nullptr);

    QLabel *label;

    void setClientModel(ClientModel *clientModel);
    void setWalletModel(WalletModel *walletModel);
signals:

public slots:
    void setBalance(qint64 balance, qint64 unconfirmedBalance, qint64 immatureBalance);
    void updateDisplayUnit();

private:
    ClientModel *clientModel;
    WalletModel *walletModel;
    int unit;
    qint64 currentBalance;
    qint64 currentUnconfirmedBalance;
    qint64 currentImmatureBalance;

protected:

    void paintEvent(QPaintEvent *e);

};

#endif // UIHEADER_H
