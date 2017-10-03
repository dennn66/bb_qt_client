#ifndef TOKEN_H
#define TOKEN_H
#include "l2graphicsobject.h"

#define TOKEN_1 0
#define TOKEN_2 1
#define TOKEN_3 0
#define TOKEN_4 0
#define TOKEN_NONE 5

#define STAR_COLOR qRgb(183, 150, 5)
#define HEART_COLOR qRgb(220, 70, 18)
#define MOON_COLOR qRgb(28, 133, 222)
#define CROSS_COLOR qRgb(72, 32, 72)

class Token : public L2GraphicObject
{
public:
    explicit Token(L2GraphicObject *parent = 0);

    /*
    void findToken(QImage image, QPoint topleft, QPoint rightbottom){
        if((startopleft.rx()<0 && getTimer()) ){

            startopleft = findPattern(image, topleft, rightbottom, star, 5);
            qDebug("startopleft %d %d", startopleft.rx(), startopleft.ry());
        }
    }
    */

    void findToken(QPoint topleft){
        if(topleft.x() > 0 && topleft.y() > 0) startopleft = topleft;
            qDebug("startopleft %d %d", startopleft.rx(), startopleft.ry());
    }

    QColor* getTokenColor(){
    //   qDebug("L2Window::getStarColor");
        return &token_color;
    }

    int getTokenState(){
    //   qDebug("L2Window::getStarState");
        return nToken;
    }
    void reset(){
       startopleft = QPoint(-1, -1);

    }
    void checkToken(QImage* image) {

        if(startopleft.rx() > 0 ){
            QImage icotmp2=image->copy(startopleft.rx()+3, startopleft.ry()+4, 6, 6);
            for(int j=0;j<6;j++) {
                for(int i=0;i<6;i++){
                     QRgb pix = icotmp2.pixel(QPoint(i,j));
                     QColor c1(pix);
                     if((j == 0) && (i == 0)) token_color=c1;
                     token_color.setRed((token_color.red()+c1.red())/2);
                     token_color.setGreen((token_color.green()+c1.green())/2);
                     token_color.setBlue((token_color.blue()+c1.blue())/2);
                }
            }
            if(CompareColors(token_color.rgb(),STAR_COLOR, 5)) {nToken = TOKEN_1;}
            else if(CompareColors(token_color.rgb(),HEART_COLOR, 5)) {nToken = TOKEN_2;}
            else if(CompareColors(token_color.rgb(),MOON_COLOR, 5)) {nToken = TOKEN_3;}
            else if(CompareColors(token_color.rgb(),CROSS_COLOR, 5)) {nToken = TOKEN_4;}
            else  {nToken = TOKEN_NONE;}

        } else {
            token_color.setRgb(qRgb(128, 128, 128));
            nToken = TOKEN_NONE;
        }

    }

protected:
//    QImage star;
    QPoint startopleft;

    int nToken;
    QColor token_color; //token
};

#endif // TOKEN_H
