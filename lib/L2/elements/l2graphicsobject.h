#ifndef L2GRAPHICSOBJECT_H
#define L2GRAPHICSOBJECT_H

#include <QObject>
#include <QImage>
#include <QtWinExtras/QtWin>

namespace L2 {
class L2GraphicObject  : public QObject
{
public:
    L2GraphicObject(QObject *parent = 0);
    QPoint findPattern(QImage* source, QRect sercharea, QImage* pattern, int delta);

    static bool CompareColors(QRgb color1, QRgb color2, UINT8 delta, bool mode = true);
    bool findPixel(QImage* image,  int yCoord, int pix_num, UINT8 pixel_delta, QRgb color, UINT8 color_delta);
    static bool checkPattern(QPoint topleft, QImage* image, QImage* pattern, int num, int treshold, int deviation);
    void setTimer(int count){detectmax = count;}
    bool getTimer(){
        bool res;
        res = (detectcounter == 0);
        detectcounter = (++detectcounter > detectmax)?0:detectcounter;
        return res;
    }
protected:
    int detectmax;
    int detectcounter;
};
}
#endif // L2GRAPHICSOBJECT_H
