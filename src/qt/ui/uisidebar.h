#ifndef UISIDEBAR_H
#define UISIDEBAR_H

#include "uihelper.h"

#include <QWidget>

#include <QDebug>
#include <QLabel>
#include <QPainter>
#include <QPushButton>

#include <QPixmap>

class PixmapEffect
{
public:
    static QPixmap tint(const QPixmap ressource, uint tint);
};



class UIIconLabel : public QWidget
{
    Q_OBJECT
public:
    explicit UIIconLabel(QString pixmap_path_ = "", QString str_ = "", QWidget *parent = nullptr);

    QPixmap *icon_pixmap;
    QString str;
    QString pixmap_path;
    QLabel *label;

    bool active;
    bool hover;
    void setActive(bool active_);

    int index;

signals:
    void released(int);
    void resetAllButtonsExcept(int);

protected:
    void paintEvent(QPaintEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);
};



class UISideBar : public QWidget
{
    Q_OBJECT
public:
    explicit UISideBar(QWidget *parent = nullptr);

    struct menu_descriptor{
        QString pixmap_path;
        QString lable_text;
    };

    QVector<menu_descriptor> main_menu_data;
    QVector<QVector<menu_descriptor> > sub_menu_data;

    QList<UIIconLabel*> label;

signals:
    void sideBarClick(int i);

public slots:
    void emitClick(int index);
    void resetAllButtonsExcept(int index);
protected:
    void paintEvent(QPaintEvent *e);

};

#endif // UISIDEBAR_H
