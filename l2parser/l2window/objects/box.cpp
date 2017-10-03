#include "box.h"

Box::Box(L2GraphicObject *parent) : L2GraphicObject(parent)
{
    //left = NULL;
    //right = NULL;
    reset();
}



void Box::findBarbox(QImage* image, QImage* leftpattern,  QImage* rightpattern){
    if(topleft.rx()<0){
        topleft = findPattern(image, searchArea.topLeft(), searchArea.bottomRight(), leftpattern, ERR_DELTA);
        qDebug("Box::boxtopleft %d %d", topleft.rx(), topleft.ry());
    }

    if((topright.rx()<0) && (topleft.rx()>0)){
        topright = findPattern(image, QPoint(topleft.rx()+minimalwidth, topleft.ry()-10), QPoint(topleft.rx()+maxmalwidth, topleft.ry()+10), rightpattern, ERR_DELTA);
        qDebug("Box::boxtopright %d %d", topright.rx(), topright.ry());
    }
}

bool Box::findLeft(QImage* image, QString fimage){
    if(topleft.rx()<0){
        QImage  pattern;
        pattern.load(fimage);
        topleft = findPattern(image, searchArea.topLeft(), searchArea.bottomRight(), &pattern, ERR_DELTA);
        if(topleft.rx()>0) left = pattern;
        qDebug("Box::findLeft %d %d", topleft.rx(), topleft.ry());
    }
    return topleft.rx()>0;
}

bool Box::findRight(QImage* image, QString fimage){
    if((topright.rx()<0) && (topleft.rx()>0)){
        QImage  pattern;
        pattern.load(fimage);
        topright = findPattern(image, QPoint(topleft.rx()+minimalwidth, topleft.ry()-10), QPoint(topleft.rx()+maxmalwidth, topleft.ry()+10), &pattern, ERR_DELTA);
        if(topright.rx()>0) right = pattern;
        qDebug("Box::findRight %d %d", topright.rx(), topright.ry());
    }
    return topright.rx()>0;
}


void Box::drawStatus(QImage* imgStatus, QRect r){
    QPainter* painter = new QPainter();
    painter->begin(imgStatus);

    if(getLeftStatus()){
        painter->setPen(QPen(QColor("#8800FF00")));
        painter->setBrush(QBrush(QColor("#8800FF00"), Qt::SolidPattern));
    } else {
        painter->setPen(QPen(QColor("#88FF0000")));
        painter->setBrush(QBrush(QColor("#88FF0000"), Qt::SolidPattern));
    }
    painter->drawRect(QRect(r.x(),r.y(),r.width()/2,r.height()));
    if(getRightStatus()){
        painter->setPen(QPen(QColor("#8800FF00")));
        painter->setBrush(QBrush(QColor("#8800FF00"), Qt::SolidPattern));
    } else {
        painter->setPen(QPen(QColor("#88FF0000")));
        painter->setBrush(QBrush(QColor("#88FF0000"), Qt::SolidPattern));
    }
    painter->drawRect(QRect(r.x()+r.width()/2,r.y(),r.width() - r.width()/2,r.height()));

    painter->end();
    delete painter;
}
