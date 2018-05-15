#ifndef BOX_H
#define BOX_H

#include <QtGui>
#include "elements/l2graphicsobject.h"


namespace L2 {
class Box  : public L2GraphicObject
{
public:
    explicit Box(L2GraphicObject *parent = 0);

    bool getState(){return !((topleft.rx()< 0) || (topright.rx()< 0 ));}
    bool getLeftStatus(){return (topleft.rx() > 0);}
    bool getRightStatus(){return (topright.rx() > 0);}
    QRect getBoxRect()
    {
        if(getState())
        {
            return QRect(topleft.rx(), topleft.ry(), topright.rx()-topleft.rx(),left.height()) ;
        } else if (getLeftStatus()){
            return QRect(topleft.rx(), topleft.ry(), left.width(),left.height()) ;
        } else {
            return QRect(0,0,0,0);
        }
    }
    void reset(){
       detectcounter = 0;
       topleft = QPoint(-1, -1);
       topright = QPoint(-1, -1);
    }
    void setSearchArea(QRect area, int minw, int maxw){searchArea = area; minimalwidth = minw; maxmalwidth = maxw;}
    QRect getLeftSearchArea(){return searchArea;}
    QRect getRightSearchArea(){return QRect(QPoint(topleft.rx()+minimalwidth, (topleft.ry()>5)?topleft.ry()-5:0), QPoint(topleft.rx()+maxmalwidth, topleft.ry()+10));}
    bool findLeft(QImage* image, QString fimage);
    bool findRight(QImage* image, QString fimage);
    QRect getLeftMargin(){return QRect(topleft.x()-2, topleft.y(),2,20);}
    QRect getRightMargin(){return QRect(topright.x(), topright.y(),2,20);}

    bool checkBoxExist(QImage* image) {
        QImage left_partial = left.copy(0, 0, left.width(),left.width());
        return checkPattern(topleft, image, &left_partial,10, 3, 5);
    }
    void drawStatus(QImage* imgStatus, QRect r);
    void drawOverlayedStatus(QPainter* p, QPen* skillpen);


protected:
    QString caption;

    QRect searchArea; // Serch area for top left corner of the box
    int minimalwidth;
    int maxmalwidth;
    int iErrDelta;

    QImage left;
    QImage right;

    QPoint topleft;
    QPoint topright;

private:

};
}
#endif // BOX_H
