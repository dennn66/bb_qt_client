#ifndef PETBOX_H
#define PETBOX_H
#include "elements/barbox.h"
namespace L2 {
class Petbox : public Barbox
{
public:
    Petbox(Barbox *parent = 0);

    void initBars(){
        for(int i = 0; i < barlist.size(); i++ ){
            initbar(barlist[i]->getbarID(),  i);
        }
    }

    int initbar(int id,  int yposition){
//        int index = Barbox::initbar(id, 20, -11, 28, 8, yposition);
        int index = Barbox::initbar(id, 19, 12, 31, 50, yposition);
        if(index >= 0){
            barlist[index]->patternMethod = true;
            barlist[index]->barpattern = mobhp;

        }
        return index;
    }


    void reset(){
       Barbox::reset();
       pets = 0;
    }

    void checkPets(QImage* image) {
        if(!checkBoxExist(image)){
           pets = 0;
        } else {
            //targettype = TARGETMOB;
            //return;
            XPBar * hp1 = getXPBar(idPet1HP);
            XPBar * hp2 = getXPBar(idPet2HP);
            if(hp1 != NULL && hp2 != NULL){
                hp1->checkXPBar(image);
                hp2->checkXPBar(image);
                if(hp2->getXP() < XP_ERR){
                    pets = 2;
                } else{
                    pets = 1;
                }

            } else {
                pets = 0;
            }
        }
    }
    int  getPetsNum(){return pets;}

    void drawStatus(QImage* imgStatus, QRect r);

protected:
    int pets = 0;
    QImage mobhp;

};
}
#endif // PETBOX_H
