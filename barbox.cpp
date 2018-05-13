#include "barbox.h"

Barbox::Barbox(Box *parent) : Box( parent)
{

}


void Barbox::drawOverlayedStatus(QPainter* p, QPen* skillpen){
    Box::drawOverlayedStatus(p, skillpen);
    if(getLeftStatus()&&getRightStatus())
    {
        skillpen->setWidth(1);
        for(int mmb = 0; mmb < barlist.size(); mmb++){
            skillpen->setColor(QColor("#3300FF00"));
            p->setPen(*skillpen);
            int xp = barlist[mmb]->getXP();
            if(xp >=0 && xp <= 100){
                QRect xpr = barlist[mmb]->getRect();
                int x = xpr.x()+(xpr.width()*xp)/100;
                int y = xpr.y()+xpr.height()/2;
                p->drawRect(QRect(x-1,y-3,3,6));
            } else {
                skillpen->setColor(QColor("#33888888"));
                p->setPen(*skillpen);
            }
            //p->drawRect(getBoxRectByIndex(mmb));
        }
    }
}
