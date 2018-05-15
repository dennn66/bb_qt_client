#include "targetbox.h"
using namespace L2;
TargetBox::TargetBox()
{
    //LOAD CONFIG BB.ini
    QSettings sett("bb.ini", QSettings::IniFormat);

    range_width = sett.value("MAIN/RangeWidth", 500).toInt();
    range_height = sett.value("MAIN/RangeHeight", 400).toInt();
    range_top = sett.value("MAIN/RangeTop", -350).toInt();
}
