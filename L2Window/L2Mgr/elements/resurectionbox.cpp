#include "resurectionbox.h"
using namespace L2;
ResurrectionBox::ResurrectionBox(Box *parent) : Box( parent)
{
    mousex = 0;
    mousey = 0;
    bOkDetected = false;
    bInFocus = false;
    caption = "Ressurection Box";

}
