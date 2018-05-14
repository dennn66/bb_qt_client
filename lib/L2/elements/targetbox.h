#ifndef TARGETBOX_H
#define TARGETBOX_H
#include "elements/box.h"
namespace L2 {
class TargetBox : public Box
{
public:
    TargetBox();
    QRect getRange(int wingow_width, int wingow_height){return QRect((wingow_width-range_width)/2,wingow_height/2+range_top,range_width,range_height);}
    void setRange(int top, int width, int height)
    {
        range_width = width;
        range_height = height;
        range_top = top;
    }
private:
    int range_width;
    int range_height;
    int range_top;

};
}
#endif // TARGETBOX_H
