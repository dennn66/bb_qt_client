#ifndef PARTYBOX_H
#define PARTYBOX_H
#include "barbox.h"

class Partybox : public Barbox
{
public:
    Partybox(Barbox *parent = 0);

    void initBars(){
        for(int i = 0; i < barlist.size(); i++ ){
            initbar(barlist[i]->getbarID(),  i);
        }
    }

    int initbar(int id,  int yposition){
//        int index = Barbox::initbar(id, 20, -11, 28, 8, yposition);
        int index = Barbox::initbar(id, 17, -3, 26, 46, yposition);
        if(index >= 0){
            barlist[index]->patternMethod = true;
            barlist[index]->barpattern = memberhp;

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

};

#endif // PARTYBOX_H
