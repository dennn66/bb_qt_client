#include "partybox.h"
using namespace L2;
Partybox::Partybox( Barbox *parent) : Barbox( parent)
{
    memberhp.load("patterns\\mobhp.bmp");
    membermp.load("patterns\\mobmp.bmp");
    membermp.load("patterns\\membercp.bmp");

    iErrDelta = 10;


    appendbar(idPartyCP, PartyCP_COLOR, BK_PartyCP_COLOR);
    appendbar(idParty2CP,PartyCP_COLOR, BK_PartyCP_COLOR);
    appendbar(idParty3CP,PartyCP_COLOR, BK_PartyCP_COLOR);
    appendbar(idParty4CP,PartyCP_COLOR, BK_PartyCP_COLOR);
    appendbar(idParty5CP,PartyCP_COLOR, BK_PartyCP_COLOR);
    appendbar(idParty6CP,PartyCP_COLOR, BK_PartyCP_COLOR);

    appendbar(idPartyHP,MobHP_COLOR, BK_MobHP_COLOR);
    appendbar(idParty2HP,MobHP_COLOR, BK_MobHP_COLOR);
    appendbar(idParty3HP,MobHP_COLOR, BK_MobHP_COLOR);
    appendbar(idParty4HP,MobHP_COLOR, BK_MobHP_COLOR);
    appendbar(idParty5HP,MobHP_COLOR, BK_MobHP_COLOR);
    appendbar(idParty6HP,MobHP_COLOR, BK_MobHP_COLOR);

    appendbar(idPartyMP,MobMP_COLOR, BK_MobMP_COLOR);
    appendbar(idParty2MP,MobMP_COLOR, BK_MobMP_COLOR);
    appendbar(idParty3MP,MobMP_COLOR, BK_MobMP_COLOR);
    appendbar(idParty4MP,MobMP_COLOR, BK_MobMP_COLOR);
    appendbar(idParty5MP,MobMP_COLOR, BK_MobMP_COLOR);
    appendbar(idParty6MP,MobMP_COLOR, BK_MobMP_COLOR);



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
