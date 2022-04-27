#include "networkpage.h"
#include "ui_networkpage.h"

NetworkPage::NetworkPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NetworkPage)
{
    ui->setupUi(this);

}

NetworkPage::~NetworkPage()
{
    delete ui;
}

void NetworkPage::paintEvent(QPaintEvent *e)
{

}
