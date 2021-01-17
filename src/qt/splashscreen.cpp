#include "splashscreen.h"
#include "clientversion.h"
#include "util.h"

#include <QApplication>

#include <QDesktopWidget>

SplashScreen::SplashScreen(QWidget *parent) :
    QWidget(parent)
{

    QRect rec = QApplication::desktop()->screenGeometry();

    int screenWidth = rec.width();
    int screenHeight = rec.height();

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setGeometry(screenWidth/2-200,screenHeight/2-130,400,260);

    splashImage = new QLabel(this);
    QPixmap newPixmap;
    newPixmap.load(":/images/splash");


    splashImage->setPixmap(newPixmap);


    QFont largeFont; largeFont.setPixelSize(14);

    label = new QLabel(this);
    label->setStyleSheet("QLabel { color: #000000; }");
    label->setFont(largeFont);
    label->setText("...");
    label->setAlignment(Qt::AlignCenter);
    label->setFixedSize(400,20);
    label->move(0,230);

}

SplashScreen::~SplashScreen()
{
}
