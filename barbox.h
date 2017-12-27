#ifndef BARBOX_H
#define BARBOX_H


#include "box.h"
#include "xpbar.h"
#include "keycondition.h"



class Barbox : public Box
{

public:
    explicit Barbox(Box *parent = 0);


    int  getBoxXP(int id){
        if(!barlist.isEmpty()) for(int index = 0; index < barlist.size();index++) if(barlist[index]->getbarID() == id) return barlist[index]->getXP();
        return XP_ERR;
    }
    QRect  getBoxRect(int id){
        if(!barlist.isEmpty()) for(int index = 0; index < barlist.size();index++) if(barlist[index]->getbarID() == id) return barlist[index]->getRect();
        return QRect(0,0,0,0);
    }

    XPBar*  getXPBar(int id){
        if(!barlist.isEmpty()) for(int index = 0; index < barlist.size();index++) if(barlist[index]->getbarID() == id) return barlist[index];
        return NULL;
    }
    void reset(){
        Box::reset();
        if(!barlist.isEmpty()){
            for(int i = 0; i < barlist.size(); i++ ){
                barlist[i]->setStatus(false);
            }
        }
    }



    void checkBars(QImage* image) {
        if(!barlist.isEmpty()){
            for(int i = 0; i < barlist.size(); i++ ){
                barlist[i]->checkXPBar(image);
            }
        }
    }



    void appendbar(int id, QRgb color, QRgb bkcolor){
        XPBar* pbar = new XPBar();
        pbar->setColors(color, bkcolor);
        pbar->setbarID(id);
        pbar->patternMethod = false;
        pbar->setStatus(false);
        barlist.append(pbar);
    }

    int initbar(int id, int leftmargin, int rightmargin, int topmargin, int yinterval, int yposition){
        if(!barlist.isEmpty()){
            for(int index = 0; index < barlist.size();index++){
                if(barlist[index]->getbarID() == id){
                    barlist[index]->xBegin = topleft.rx()+leftmargin;
                    barlist[index]->xEnd = topright.rx()-rightmargin;
                    barlist[index]->yXP = topleft.ry()+topmargin+yposition*yinterval;
                    barlist[index]->setStatus(true);
                    return index;
                }
            }
        }
        return -1;
    }

protected:
    QVector <XPBar*> barlist;


signals:

public slots:
};

#endif // BARBOX_H
