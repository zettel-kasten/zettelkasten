#ifndef MININGPAGE_H
#define MININGPAGE_H

#include <QWidget>
#include <memory>

#include "walletmodel.h"

namespace Ui {
class MiningPage;
}

class MiningPage : public QWidget
{
    Q_OBJECT

public:
    explicit MiningPage(QWidget *parent = 0);
    ~MiningPage();

    void setModel(WalletModel *model);

private:
    Ui::MiningPage *ui;
    WalletModel *model;
    std::auto_ptr<WalletModel::UnlockContext> unlockContext;
    bool hasMiningprivkey;

    void restartMining(bool fGenerate);
    void timerEvent(QTimerEvent *event);
    void updateUI();

private slots:

    void changeNumberOfCores(int i);
    void switchMining();
};

#endif // MININGPAGE_H
