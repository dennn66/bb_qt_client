#ifndef BOX_H
#define BOX_H

#include <QtGui>
#include "l2graphicsobject.h"

#define ERR_DELTA 5

class Box  : public L2GraphicObject
{
public:
    explicit Box(L2GraphicObject *parent = 0);

    bool state(){return !((topleft.rx()< 0) || (topright.rx()< 0 ));}
    bool getLeftStatus(){return (topleft.rx() > 0);}
    bool getRightStatus(){return (topright.rx() > 0);}
    void reset(){
       detectcounter = 0;
       topleft = QPoint(-1, -1);
       topright = QPoint(-1, -1);
    }
    void setSearchArea(QRect area, int minw, int maxw){searchArea = area; minimalwidth = minw; maxmalwidth = maxw;}
    void findBarbox(QImage* image, QImage* leftpattern,  QImage* rightpattern);

    bool findLeft(QImage* image, QString fimage);
    bool findRight(QImage* image, QString fimage);

    bool checkBoxExist(QImage* image) {
        QImage left_partial = left.copy(0, 0, left.width(),left.width());
        return checkPattern(topleft, image, &left_partial,10, 3, 5);
    }
    void drawStatus(QImage* imgStatus, QRect r);


protected:
    QRect searchArea; // Serch area for top left corner of the box
    int minimalwidth;
    int maxmalwidth;

    QImage left;
    QImage right;

    QPoint topleft;
    QPoint topright;

private:

};

#endif // BOX_H
