#include "l2parser.h"

L2parser::L2parser(QObject *parent) : QObject(parent)
{
    currentl2w = NULL;

}

void L2parser::process()
{
    qDebug("Start l2 checker");

    while(1){
        QTime updateTime;
        updateTime.start();
        int delta;

        if(currentl2w != NULL) {
            try {
                currentl2w->check();

                QRect l2rect = currentl2w->getL2WRect();

                QImage* clicker_bk = new QImage(QSize(l2rect.width(),l2rect.height()), QImage::Format_ARGB32);
                if(!clicker_bk->isNull()){
                    currentl2w->getStatusBk(clicker_bk);
                }
                for(int i = 0; i < KEYNUM; i++){  //48 keys
                    if(currentl2w->is_dongle_skill_state_changed(i) ) emit set_dongle_skill_state(i, currentl2w->get_dongle_skill_state(i));
                    //if(currentl2w->is_visual_skill_state_changed(i) )
                    emit set_visual_skill_state(i, currentl2w->get_visual_skill_state(i), currentl2w->getConditionState(i), currentl2w->get_visual_skill_group_state(i));

                }

                emit showParserStatus(updateTime.elapsed(), currentl2w, *clicker_bk);
                if(!clicker_bk->isNull()){
                    delete clicker_bk;
                }


            } catch(...) {
                qDebug("Check failed");
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


void L2parser::setActiveL2W(L2Window* l2w)
{
    qDebug("L2parser::setActiveL2W");
    currentl2w = l2w;
}


 void L2parser::setGroupState(int num,  bool state){
     if(currentl2w != NULL) {
         currentl2w->setGroupState(num, state);
         if(currentl2w->is_group_state_changed(num)) emit updateGroupState(num, currentl2w->get_group_state(num));
     } else {
         emit updateGroupState(num,  false);
     }
}

void L2parser::redraw(){
    for(int i = 0; i < KEYNUM; i++){  //48 keys
        emit set_dongle_skill_state(i, false);
        emit set_visual_skill_state(i, false, false, false);
    }
    for(int i = 0; i < GROUPSNUM; i++){  //4
        emit updateGroupState(i, false);
    }
}
