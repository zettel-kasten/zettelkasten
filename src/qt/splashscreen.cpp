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
    this->setGeometry(screenWidth/2-200,screenHeight/2-12,400,260);


    //QPixmap bgPixmap(320,360);

    /*QLinearGradient bgGradient(QPointF(0, 0), QPointF(screenWidth, 0));
    bgGradient.setColorAt(0, QColor("#FEFEFE"));//#6c3d94"));
    bgGradient.setColorAt(1, QColor("#FEFEFE"));//"#a13469"));
    */
    //#3c3c3b

 /*   QRect rect_linear(0,0,320,360);

    QPainter *painter = new QPainter(&bgPixmap);
    painter->fillRect(rect_linear, bgGradient);

    painter->end();

    bg = new QLabel(this);
    bg->setPixmap(bgPixmap);


    bg->setGeometry(0,0,320,360);
*/
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
