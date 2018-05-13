#include "l2engine.h"

L2engine::L2engine(QObject *parent) : QObject(parent)
{
    currentl2w = NULL;

}

void L2engine::process()
{
    qDebug("Start l2 checker");

    while(1){
        QTime updateTime;
        updateTime.start();
        int delta;

        if(currentl2w != NULL) {
            try {
                currentl2w->l2parser->check();
                emit isL2Active( currentl2w->l2parser->isActiveL2Window, currentl2w->l2parser->windowtopright.rx(), currentl2w->l2parser->windowtopright.ry());
                emit showParserStatus(updateTime.elapsed(), currentl2w);
            } catch(...) {
                qDebug("Check failed");
                emit isL2Active( false, 0, 0);
            }

        }
        delta = 50 - updateTime.elapsed();
        delta = (delta > 0)?delta:0;
        #ifdef WIN32
            Sleep(delta);
        #else
            usleep(delta*1000);
        #endif

    }
    emit finished();
}


void L2engine::setActiveL2W(L2Window* l2w)
{
    qDebug("L2parser::setActiveL2W");
    currentl2w = l2w;
}

void L2engine::doActivateL2()
{
    qDebug("L2parser::doActivateL2W()");
    if(currentl2w != NULL)    SetActiveWindow(currentl2w->l2parser->getHWND());
}


