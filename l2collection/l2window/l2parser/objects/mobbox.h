#ifndef MOBBOX_H
#define MOBBOX_H
#include "barbox.h"
#include "token.h"
//#include "mobbar.h"

#define TARGETMEORPET   0
#define TARGETCHAR      1
#define TARGETMOB       2
#define NOTARGET        3
#define TARGETSTATESNUM 4

class Mobbox : public Barbox
{
public:
    Mobbox(Barbox *parent = 0);

    void initBars(){
        for(int i = 0; i < barlist.size(); i++ ){
            initbar(barlist[i]->getbarID(),  i);
        }
    }

    int initbar(int id,  int yposition){
        int index = Barbox::initbar(id, 21, -12, 28, 8, yposition);
        if(index >= 0){
            barlist[index]->patternMethod = true;
            if(id == idMobHP) {
                barlist[index]->barpattern = mobhp;
            } else {
                barlist[index]->barpattern = mobmp;
            }
        }
        return index;
    }

    void findToken(){

//        if(state()) token.findToken(image, QPoint(topleft.x()+20, topleft.y()+6), QPoint(topleft.x()+24, topleft.y()+10));
        if(state()){

            token.findToken(QPoint(topleft.x()+22, topleft.y()+8));


        }

        //XPBar * hp = getXPBar(idMobHP);
        //if(hp != NULL ){
        //    token.findToken(image, QPoint(hp->getBegin(), hp->getY()-23), QPoint(hp->getBegin()+19, hp->getY()-5));
        //}
    }

    void reset(){
       Barbox::reset();
       targettype = NOTARGET;
    }

    void checkTargetType(QImage* image) {
        if(!checkBoxExist(image)){
           targettype = NOTARGET;
        } else {
            //targettype = TARGETMOB;
            //return;
            XPBar * hp = getXPBar(idMobHP);
            XPBar * mp = getXPBar(idMobMP);
            if(hp != NULL && mp != NULL){
                mp->checkXPBar(image);
                if(mp->getXP() < XP_ERR) {
    //                getXPBar(idMobHP)->setXP(getXPBar(idHP)->getXP());
                    hp->checkXPBar(image);
                    targettype = TARGETMEORPET;
                } else {
                    hp->checkXPBar(image);
                    if(hp->getXP() < XP_ERR) {
                        targettype = TARGETMOB;
                    } else {
                        targettype = TARGETCHAR;
                    }
                }
            }
        }
    }
    int  getTargetType(){return targettype;}

    QColor* getTokenColor(){
    //   qDebug("L2Window::getStarColor");
        return token.getTokenColor();
    }

    int getTokenState(){
    //   qDebug("L2Window::getStarState");
        return token.getTokenState();
    }
    void checkToken(QImage* image) {
        if(((targettype == TARGETCHAR)||(targettype  == TARGETMOB)) ){
            token.checkToken(image);
        }
    }

    void drawStatus(QImage* imgStatus, QRect r);

protected:
    int targettype;
    Token token;

    QImage mobhp;
    QImage mobmp;
};

#endif // MOBBOX_H
