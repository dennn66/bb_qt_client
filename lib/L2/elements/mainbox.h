#ifndef MAINBOX_H
#define MAINBOX_H
#include "elements/barbox.h"
//#include "mainbar.h"
namespace L2 {
class Mainbox  : public Barbox
{
public:
    Mainbox(Barbox *parent = 0);
    QImage copyIcon(QImage* image){return image->copy(topleft.rx()+40, topleft.ry()+3, 18, 18);}

    void initBars(){
        if(!barlist.isEmpty()){
            for(int i = 0; i < barlist.size(); i++ ){
                initbar(barlist[i]->getbarID(),  i);
            }
        }
    }
    int initbar(int id,  int yposition){
        int index = Barbox::initbar(id, 19, -3, 32, 13, yposition);
        if(index >= 0) barlist[index]->patternMethod = false;
        return index;
    }
protected:


};
}
#endif // MAINBOX_H
