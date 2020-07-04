#ifndef UIFOOTER_H
#define UIFOOTER_H

#include "uihelper.h"
#include "clientmodel.h"

#include <QLabel>

class UIFooter : public QWidget
{
    Q_OBJECT
public:
    explicit UIFooter(QWidget *parent = nullptr);

    QList<QLabel*> label;

    int numConnections;
    int count;
    int nTotalBlocks;
    QStringList progress_str_list;
    int secs;

    bool show_preloader;

    BlockSource blockSource;
signals:

public slots:
    void updateDataLabels();
    void setNumConnections(int num);
    void setProgressInfo(QStringList str_list, int _count, int _nTotalBlocks, int _secs, BlockSource _blockSource);
protected:
    void paintEvent(QPaintEvent *e);
};

#endif // UIFOOTER_H
