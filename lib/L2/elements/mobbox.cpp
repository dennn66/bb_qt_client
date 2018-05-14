#include "mobbox.h"
using namespace L2;
Mobbox::Mobbox( Barbox *parent) : Barbox( parent)
{
    mobhp.load("patterns\\mobhp.bmp");
    mobmp.load("patterns\\mobmp.bmp");

    appendbar(idMobHP,MobHP_COLOR, BK_MobHP_COLOR);
    appendbar(idMobMP,MobMP_COLOR, BK_MobMP_COLOR);
    caption = "Mob Box";

    initBars();
}

void Mobbox::drawStatus(QImage* imgStatus, QRect r){

    if(getLeftStatus() && getRightStatus()){
        QPainter p;
        p.begin(imgStatus);
        if(getTargetType() == TARGETMEORPET) {
            p.setPen(QPen(QColor("#8800FFFF")));
            p.setBrush(QBrush(QColor("#8800FFFF"), Qt::SolidPattern));

        } else if(getTargetType() == NOTARGET){
            p.setPen(QPen(QColor("#88888888")));
            p.setBrush(QBrush(QColor("#88888888"), Qt::SolidPattern));

        } else if(getTargetType() == TARGETCHAR){
            p.setPen(QPen(QColor("#88FF00FF")));
            p.setBrush(QBrush(QColor("#88FF00FF"), Qt::SolidPattern));

        } else if(getTargetType() == TARGETMOB){
            p.setPen(QPen(QColor("#880000FF")));
            p.setBrush(QBrush(QColor("#880000FF"), Qt::SolidPattern));

        }
        p.drawRect(r);
        p.end();
    } else {
        Box::drawStatus(imgStatus, r);
    }

}
