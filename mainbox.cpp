#include "mainbox.h"

Mainbox::Mainbox( Barbox *parent) : Barbox( parent)
{
    appendbar(idCP,CP_COLOR, BK_CP_COLOR);
    appendbar(idHP,HP_COLOR, BK_HP_COLOR);
    appendbar(idMP,MP_COLOR, BK_MP_COLOR);
    appendbar(idVP,VP_COLOR, BK_VP_COLOR);
    initBars();
}
