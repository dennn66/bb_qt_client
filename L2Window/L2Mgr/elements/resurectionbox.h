#ifndef RESURECTIONBOX_H
#define RESURECTIONBOX_H
#include "elements/box.h"

#define OK_LEFT_OFFSET 52
#define OK_TOP_OFFSET 91
#define OK_WIDTH 70
#define OK_HEIGHT 17

#define USB_MOUSE_MAX_DX 30
#define USB_MOUSE_MIN_DX 8
#define USB_MOUSE_DIV 5


namespace L2 {
class ResurrectionBox : public Box
{
public:
    explicit ResurrectionBox(Box *parent = 0);
    QRect getOkRect(){return QRect(topleft.rx()+OK_LEFT_OFFSET, topleft.ry()+OK_TOP_OFFSET, OK_WIDTH, OK_HEIGHT);}  //47+62-30-17-10,  47+62-17-2,  73,  21
    void setOkDetected(bool state){bOkDetected = state;}
    bool getOkDetected(){return bOkDetected;}
    bool isOkInFocus (     )
    {
        return (getDeltaX() == 0 && getDeltaY() == 0);
    }

    qint8 getDeltaX()
    {
        if(mousex < topleft.rx()+OK_LEFT_OFFSET)
        {
            return normalize((topleft.rx()+OK_LEFT_OFFSET + OK_WIDTH/2) - mousex);
        } else if(mousex > topleft.rx()+OK_LEFT_OFFSET + OK_WIDTH) {
            return normalize((topleft.rx()+OK_LEFT_OFFSET + OK_WIDTH/2) - mousex);
        } else {
            return 0;
        }
    }
    qint8 getDeltaY()
    {
        if(mousey < (topleft.ry()+OK_TOP_OFFSET))
        {
            return normalize((topleft.ry()+OK_TOP_OFFSET+OK_HEIGHT/2) - mousey);
        } else if(mousey > (topleft.ry()+OK_TOP_OFFSET+OK_HEIGHT)) {
            return normalize((topleft.ry()+OK_TOP_OFFSET+OK_HEIGHT/2) - mousey);
        } else {
            return 0;
        }
    }
    void setMouseCoord(int x, int y)    {mousex = x;   mousey = y;}
    QPoint getMouseCoord() {return QPoint(mousex, mousey);}



private:
    qint8 normalize(int x){
        if(x > 0){
            x /= USB_MOUSE_DIV;
            x = (x == 0)?USB_MOUSE_MIN_DX:x;
            x = (x > USB_MOUSE_MAX_DX)? USB_MOUSE_MAX_DX: x;
        } else if(x < 0) {
            x /= USB_MOUSE_DIV;
            x = (x == 0)?-USB_MOUSE_MIN_DX:x;
            x = (x < USB_MOUSE_MAX_DX)? -USB_MOUSE_MAX_DX: x;
        }
        return x;
    }

    int  mousex;
    int  mousey;
    bool bOkDetected;
    bool bInFocus;

};
}
#endif // RESURECTIONBOX_H
