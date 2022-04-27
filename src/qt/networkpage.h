#ifndef NETWORKPAGE_H
#define NETWORKPAGE_H

#include <QWidget>

namespace Ui {
class NetworkPage;
}

class NetworkPage : public QWidget
{
    Q_OBJECT
public:
    explicit NetworkPage(QWidget *parent = nullptr);
    ~NetworkPage();

    void paintEvent(QPaintEvent *e);

signals:

private:
    Ui::NetworkPage *ui;

public slots:
};

#endif // NETWORKPAGE_H
