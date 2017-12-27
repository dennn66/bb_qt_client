#include "l2graphicsobject.h"

L2GraphicObject::L2GraphicObject(QObject *parent) : QObject(parent)
{

}


// Compare random points between image and pattern
QPoint L2GraphicObject::findPattern(QImage* source, QPoint topleft, QPoint bottomright, QImage* pattern, int delta){
    qDebug("QPoint L2GraphicObject::findPattern(QImage* source  = (%d, %d), QPoint topleft = (%d, %d), QPoint bottomright = (%d, %d), QImage* pattern = (%d, %d), int delta = %d)", source->width(), source->height() , topleft.x() , topleft.y() , bottomright.x() , bottomright.y() , pattern->width(), pattern->height() , delta);
    if(topleft.rx() <0 ) topleft.setX(0);
    if(topleft.ry() <0 ) topleft.setY(0);
    if(bottomright.rx() < topleft.rx() ) bottomright.setX(topleft.rx());
    if(bottomright.ry() < topleft.ry() ) bottomright.setY(topleft.ry());
    for(int im_w = topleft.rx(); im_w < bottomright.rx() - delta; im_w++ ) {
        for(int im_h = topleft.ry(); im_h < bottomright.ry() - delta; im_h++ ) {
            int count_total = 0;
            int count_match = 0;

            for(int w = 0; w < pattern->width();w++){
                for(int h = 0; h < pattern->height();h++){
                    //qDebug("Read pattern");
                    QRgb frame_pix = pattern->pixel(QPoint(w, h));

                    QColor ct(frame_pix);
                    if(!(ct.red() == 255 && ct.green()==255 && ct.blue() == 255)){
                        count_total++;;
                         QRgb image_pix = source->pixel(QPoint(im_w+w, im_h+h));
                         bool res = (frame_pix == image_pix);
                         count_match += res;
                    }
                    if(count_total - count_match >= delta) break;
                }
                if(count_total - count_match >= delta) break;
            }
            if(count_total - count_match < delta){
                return(QPoint(im_w, im_h));
            }
        }
    }
    return(QPoint(-1,-1));
}


//Compare two colors with 'delta'
bool L2GraphicObject::CompareColors(QRgb color1, QRgb color2, UINT8 delta, bool mode)
{
    //qDebug("%d XPBar::CompareColors", barID);
    QColor c1(color1);
    QColor c2(color2);
    if(mode) {
        int deviation_h = (c1.hsvHue() > c2.hsvHue())? c1.hsvHue() - c2.hsvHue() : c2.hsvHue() - c1.hsvHue();
        deviation_h = (deviation_h > 179)? 360 - deviation_h : deviation_h;
        //qDebug("deviation_h: %d", deviation_h);
        int deviation_s;
        //qDebug("c1.hsvSaturation(): %d", c1.hsvSaturation());
        //qDebug("c2.hsvSaturation(): %d", c2.hsvSaturation());
        //qDebug("c1.hsvSaturation() > c2.hsvSaturation(): %d", c1.hsvSaturation() > c2.hsvSaturation());

        deviation_s = (c1.hsvSaturation() > c2.hsvSaturation())? c1.hsvSaturation() - c2.hsvSaturation() : c2.hsvSaturation() - c1.hsvSaturation();
        //qDebug("deviation_s: %d", deviation_s);

        return (deviation_h < delta) && (deviation_s < delta);
    } else {
        int deviation_r = (c1.red() > c2.red())? c1.red() - c2.red() : c2.red() - c1.red();
        int deviation_g = (c1.green() > c2.green())? c1.green() - c2.green() : c2.green() - c1.green();
        int deviation_b = (c1.blue() > c2.blue())? c1.blue() - c2.blue() : c2.blue() - c1.blue();

        return (deviation_r <delta) && (deviation_g <delta) && (deviation_b <delta);
    }
}

//Find pixel by color
bool L2GraphicObject::findPixel(QImage* image, int yCoord, int pix_num, UINT8 pixel_delta, QRgb color, UINT8 color_delta)
{
    //qDebug("%d XPBar::findPixel", barID);
    int starty = yCoord-pixel_delta;
    int endy=yCoord+pixel_delta;
    starty = (starty>0)?starty:0;

    for(int row = starty; row < endy; row++){
        QRgb pix = image->pixel(QPoint(pix_num,row));
        if(CompareColors(pix, color, color_delta)){
            //qDebug("return true");
            return true;
        }
    }
    //qDebug("return false");
    return false;
}


// Compare random points between image and pattern
bool L2GraphicObject::checkPattern(QPoint topleft, QImage* image, QImage* pattern, int num, int treshold, int deviation){
    if(!(topleft.rx() > 0 && topleft.ry() > 0)) return false;
    int         notvalid = 0;
    for(int i=0; i<num; i ++)
    {
        int x = (pattern->width()*qrand())/RAND_MAX;
        int y = (pattern->height()*qrand())/RAND_MAX;
        QColor img_pix = image->pixelColor(QPoint(x+topleft.rx(),y+topleft.ry()));
        QColor pattern_pix = pattern->pixelColor(QPoint(x,y));
        if(!(pattern_pix.red() == 255 && pattern_pix.green() == 255 &&pattern_pix.blue() == 255)){
            int deviation_r = (img_pix.red() > pattern_pix.red())? img_pix.red() - pattern_pix.red() : pattern_pix.red() - img_pix.red();
            int deviation_g = (img_pix.green() > pattern_pix.green())? img_pix.green() - pattern_pix.green() : pattern_pix.green() - img_pix.green();
            int deviation_b = (img_pix.blue() > pattern_pix.blue())? img_pix.blue() - pattern_pix.blue() : pattern_pix.blue() - img_pix.blue();
            if( (deviation_r >deviation) || (deviation_g >deviation) || (deviation_b >deviation)) notvalid++;
            if(notvalid > treshold)  return false;
        }
    }
    return true;
}
