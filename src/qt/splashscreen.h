#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QLinearGradient>

/** class for the splashscreen with information of the running client
 */
class SplashScreen : public QWidget
{
    Q_OBJECT

public:
    explicit SplashScreen(QWidget *parent = 0);
    ~SplashScreen();

    QLabel *bg;
    QPixmap bgPixmap;

    QPainter *painter;

    QLabel *splashImage;
    QPixmap newPixmap;
    QLabel *versionLabel;
    QLabel *label;


};

#endif // SPLASHSCREEN_H
