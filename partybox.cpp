#include "partybox.h"

Partybox::Partybox( Barbox *parent) : Barbox( parent)
{
    memberhp.load("patterns\\mobhp.bmp");
    iErrDelta = 10;
    appendbar(idPartyHP,MobHP_COLOR, BK_MobHP_COLOR);
    appendbar(idParty2HP,MobHP_COLOR, BK_MobHP_COLOR);
    appendbar(idParty3HP,MobHP_COLOR, BK_MobHP_COLOR);
    appendbar(idParty4HP,MobHP_COLOR, BK_MobHP_COLOR);
    appendbar(idParty5HP,MobHP_COLOR, BK_MobHP_COLOR);
    appendbar(idParty6HP,MobHP_COLOR, BK_MobHP_COLOR);
    //barlist[0]->bEnableBarSampling = true;
    caption = "Party Box";

    initBars();
}

void Partybox::drawStatus(QImage* imgStatus, QRect r){

    if(getLeftStatus()){
        QPainter p;
        p.begin(imgStatus);
        if( getRightStatus()){

            switch(members){
                case 0:
                    p.setPen(QPen(QColor("#88888888")));
                    p.setBrush(QBrush(QColor("#88888888"), Qt::SolidPattern));
                    break;
                case 1:
                    p.setPen(QPen(QColor("#8800FFFF")));
                    p.setBrush(QBrush(QColor("#8800FFFF"), Qt::SolidPattern));
                    break;
                case 2:
                    p.setPen(QPen(QColor("#880000FF")));
                    p.setBrush(QBrush(QColor("#880000FF"), Qt::SolidPattern));
                    break;
                default:
                    p.setPen(QPen(QColor("#FF0000FF")));
                    p.setBrush(QBrush(QColor("#FF0000FF"), Qt::SolidPattern));
                    break;
            }
            p.drawEllipse(r);
        } else {
            p.setPen(QPen(QColor("#FFFF00FF")));
            p.setBrush(QBrush(QColor("#FF888800"), Qt::SolidPattern));
            p.drawRect(r);
        }

        p.end();
    } else {
        Box::drawStatus(imgStatus, r);
    }

}
