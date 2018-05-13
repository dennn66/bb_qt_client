#include "box.h"

Box::Box(L2GraphicObject *parent) : L2GraphicObject(parent)
{
    //left = NULL;
    //right = NULL;
    iErrDelta = 5;
    caption = "";
    reset();
}

bool Box::findLeft(QImage* image, QString fimage){
    if(topleft.rx()<=0){
        QImage  pattern;
        pattern.load(fimage);
        topleft = findPattern(image, searchArea, &pattern, iErrDelta);
        if(topleft.rx()>0) left = pattern;
        //qDebug() << fimage << " Box::findLeft " <<  topleft.rx() << ", " << topleft.ry();
    }
    return topleft.rx()>0;
}

bool Box::findRight(QImage* image, QString fimage){
    if((topright.rx()<0) && (topleft.rx()>0)){
        QImage  pattern;
        pattern.load(fimage);
        topright = findPattern(image, getRightSearchArea(), &pattern, iErrDelta);
        if(topright.rx()>0) right = pattern;
        //qDebug("Box::findRight %d %d", topright.rx(), topright.ry());
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

void Box::drawOverlayedStatus(QPainter* p, QPen* skillpen){
    QString cap = caption;

    p->setBrush(QBrush(QColor("#00FFFFFF"), Qt::SolidPattern));
    if(!getLeftStatus())
    {
        skillpen->setColor(QColor("#88888888"));
        p->setPen(*skillpen);
        p->drawRect(getLeftSearchArea());
        QTextStream (&cap) << " Left side";
        p->drawText(getLeftSearchArea().topLeft(), cap);
    } else {
        skillpen->setColor(QColor("#8800FF00"));
        p->setPen(*skillpen);
        //p->drawRect(getLeftMargin());

        if(!getRightStatus())
        {
            skillpen->setColor(QColor("#88888888"));
            p->setPen(*skillpen);
            p->drawRect(getRightSearchArea());
            QTextStream (&cap) << " Right side";
            p->drawText(getRightSearchArea().topLeft(), cap);

        } else {
            skillpen->setColor(QColor("#3300FF00"));
            p->setPen(*skillpen);
            //p->drawRect(getRightMargin());
        }
    }
}
