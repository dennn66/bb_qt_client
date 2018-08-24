#ifndef PARTYBOX_H
#define PARTYBOX_H
#include "elements/barbox.h"
namespace L2 {
class Partybox : public Barbox
{
public:
    Partybox(Barbox *parent = 0);

    void initBars(){
        for(int j = 0; j < 3; j++ ){

            for(int i = 0; i < 6; i++ ){
                initbar(barlist[j*6+i]->getbarID(),  i, j);
            }
        }
    }

    int initbar(int id,  int ptmember, int mmbrbar){
//        int index = Barbox::initbar(id, 20, -11, 28, 8, yposition);
        int index = Barbox::initbar(id, 17, -3, 18+8*mmbrbar, 46, ptmember);
        if(index >= 0){

            switch(mmbrbar){
            case 0:
                barlist[index]->patternMethod = false;
                barlist[index]->barpattern = membercp;
            case 1:
                barlist[index]->patternMethod = false;
                barlist[index]->barpattern = memberhp;
            case 2:
                barlist[index]->patternMethod = false;
                barlist[index]->barpattern = membermp;
            }

        }
        return index;
    }


    void reset(){
       Barbox::reset();
       members = 0;
    }

    void checkMembers(QImage* image) {
        if(!checkBoxExist(image)){
           members = 0;
        } else {
            int member_count = 0;
            for(int mmb = 0; mmb < barlist.size(); mmb++)
            {
                barlist[mmb]->checkXPBar(image);
                if(barlist[mmb]->getXP() < XP_ERR) member_count++;
            }
            members = member_count;
        }
    }

    int  getMembersNum(){return members;}

    void drawStatus(QImage* imgStatus, QRect r);

protected:
    int members = 0;
    QImage memberhp;
    QImage membermp;
    QImage membercp;

};
}
#endif // PARTYBOX_H
