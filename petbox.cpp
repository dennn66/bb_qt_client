#include "petbox.h"


Petbox::Petbox( Barbox *parent) : Barbox( parent)
{
    mobhp.load("patterns\\mobhp.bmp");

    appendbar(idPet1HP,MobHP_COLOR, BK_MobHP_COLOR);
    appendbar(idPet2HP,MobHP_COLOR, BK_MobHP_COLOR);
    caption = "Pet Box";

    initBars();
}


void Petbox::drawStatus(QImage* imgStatus, QRect r){

    if(getLeftStatus() && getRightStatus()){
        QPainter p;
        p.begin(imgStatus);
        switch(pets){
            case 1:
                p.setPen(QPen(QColor("#8800FFFF")));
                p.setBrush(QBrush(QColor("#8800FFFF"), Qt::SolidPattern));
                break;
            case 2:
                p.setPen(QPen(QColor("#880000FF")));
                p.setBrush(QBrush(QColor("#880000FF"), Qt::SolidPattern));
                break;
            default:
                p.setPen(QPen(QColor("#88888888")));
                p.setBrush(QBrush(QColor("#88888888"), Qt::SolidPattern));
                break;

        }
        p.drawRect(r);
        p.end();
    } else {
        Box::drawStatus(imgStatus, r);
    }

}
