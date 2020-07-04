#include "uisidebar.h"

QPixmap PixmapEffect::tint(const QPixmap ressource, uint tint)
{
    QImage img = ressource.toImage().convertToFormat(QImage::Format_ARGB32,Qt::AutoColor);

    uint ARGB;
    uint ARGB_tint;
    for(int x=0; x<img.width();x++){
        for(int y=0; y<img.height();y++){
            ARGB = (uint)img.pixel(x,y);
            ARGB_tint = (ARGB & 0xFF000000) + tint;
            img.setPixel(x,y, ARGB_tint);
        }
    }
    return QPixmap::fromImage(img);
}


UIIconLabel::UIIconLabel(QString pixmap_path_, QString str_, QWidget *parent) :
    QWidget(parent),
    pixmap_path(pixmap_path_),
    str(str_),
    active(false),
    hover(false)
{

    label = new QLabel(this);
    label->setText(str);
    if(pixmap_path != ""){
        label->setStyleSheet(".QLabel{color:#FFFFFF; font: bold 16px \"Open Sans\";}");
    } else{
        label->setStyleSheet(".QLabel{color:#F7F7F7; font: bold 11px \"Open Sans\";}");
    }
    label->setAlignment(Qt::AlignVCenter);
    label->setFixedHeight(20);
    label->setContentsMargins(0,0,0,0);
    label->move(55,-1);
    label->adjustSize();
}

void UIIconLabel::setActive(bool active_)
{
    active = active_;
    label->setStyleSheet(".QLabel{color:#FFFFFF; font: bold 16px \"Open Sans\";}");
    if(active){
        label->setStyleSheet(".QLabel{color:#FF7F00; font: bold 16px \"Open Sans\";}");
    }
    update();
}

void UIIconLabel::paintEvent(QPaintEvent *e)
{



    QPainter painter;

    painter.begin(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    //painter.fillRect(0,0,150,this->height(),QColor(0x333333));

    if(pixmap_path != ""){
        QPixmap pix(pixmap_path);
        if(active || hover){
            pix = PixmapEffect::tint(pix,0xFF7F00);
        } else{
            pix = PixmapEffect::tint(pix,0xFFFFFF);
        }
        painter.drawPixmap(30,0,pix);

    }


    if(active){
        painter.fillRect(0,8,20,2,QColor(0xFF7F00));
    }

    painter.end();

}

void UIIconLabel::mouseReleaseEvent(QMouseEvent *e)
{
    active=true;
    //update();
    emit resetAllButtonsExcept(index);
    emit released(index);
}

void UIIconLabel::enterEvent(QEvent *)
{
    hover = true;
    label->setStyleSheet(".QLabel{color:#FF7F00; font: bold 16px \"Open Sans\";}");
    update();
}

void UIIconLabel::leaveEvent(QEvent *)
{
    hover = false;
    if(!active){
        label->setStyleSheet(".QLabel{color:#FFFFFF; font: bold 16px \"Open Sans\";}");
    }
    update();
}

UISideBar::UISideBar(QWidget *parent) : QWidget(parent)
{
    resize(200,1000);
    setContentsMargins(0,0,0,0);

    main_menu_data.append({":/ui/icons/timeline","Timeline"});
    main_menu_data.append({":/ui/icons/wallet","Wallet"});
    main_menu_data.append({":/ui/icons/mining","Mining"});

    int offset_y = 30;
    for(int i = 0;i<main_menu_data.length();i++){

        label.append(new UIIconLabel(main_menu_data[i].pixmap_path,
                                     main_menu_data[i].lable_text,
                                     this));
        if(main_menu_data[i].pixmap_path == ""){
            offset_y -= 10;
        }
        label[i]->move(0,offset_y);
        label[i]->index = i;
        offset_y+=40;

        connect(label[i], SIGNAL(released(int)), this, SLOT(emitClick(int)));
        connect(label[i], SIGNAL(resetAllButtonsExcept(int)), this, SLOT(resetAllButtonsExcept(int)));
    }

    label[0]->setActive(true);

}

void UISideBar::emitClick(int index)
{
    emit sideBarClick(index);
}

void UISideBar::resetAllButtonsExcept(int index)
{
    for(int i = 0;i<main_menu_data.length();i++){
        label[i]->active = false;
        label[i]->setActive(false);
        label[i]->update();
    }
    label[index]->active = true;
    label[index]->setActive(true);
    label[index]->update();
}

void UISideBar::paintEvent(QPaintEvent *e)
{
    QPainter painter;

    painter.begin(this);
    painter.fillRect(0,0,200,this->height(),QColor(0x333333));
    painter.fillRect(0,0,200,2,QColor(0x3b3b3b));//3b3b3b));

    painter.end();

}
