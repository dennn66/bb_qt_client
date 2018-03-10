#include "l2parser.h"

L2parser::L2parser(QObject *parent) : QObject(parent)
{
    currentl2i = -1;
    //LOAD CONFIG BB.ini
    QSettings sett("bb.ini", QSettings::IniFormat);
    bDongleIsWorking = false;

    default_file_name = sett.value("MAIN/DefaultProject", "default.bbproj").toString();


}

void L2parser::process()
{
    qDebug("Start l2 checker");

    while(1){
        QTime updateTime;
        updateTime.start();
        int delta;
        //qDebug("Spin l2 checker");

        if(isValidL2W()) {
            try {
                getCurrentL2W()->check();

                QRect l2rect = getCurrentL2W()->getL2WRect();

                QImage* clicker_bk = new QImage(QSize(l2rect.width(),l2rect.height()), QImage::Format_ARGB32);
                if(!clicker_bk->isNull()){
                    getCurrentL2W()->getStatusBk(clicker_bk, bDongleIsWorking);
                }
                for(int i = 0; i < KEYNUM; i++){  //48 keys
                    if(getCurrentL2W()->is_dongle_skill_state_changed(i) ) emit set_dongle_skill_state(i, getCurrentL2W()->get_dongle_skill_state(i));
                }

                emit showParserStatus(updateTime.elapsed(), getCurrentL2W(), *clicker_bk);
                if(!clicker_bk->isNull()){
                    delete clicker_bk;
                }


            } catch(...) {
                qWarning("Check failed");
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


 void L2parser::setGroupState(int num,  bool state){
     if(isValidL2W()) {
         getCurrentL2W()->setGroupState(num, state);
         if(getCurrentL2W()->is_group_state_changed(num)) emit updateGroupState(num, getCurrentL2W()->get_group_state(num));
     } else {
         emit updateGroupState(num,  false);
     }
}

void L2parser::toggleGroupState(int num){
    if(isValidL2W()) setGroupState(num, !getCurrentL2W()->get_group_state(num));
}

void L2parser::redraw(){
    for(int i = 0; i < KEYNUM; i++){  //48 keys
        emit set_dongle_skill_state(i, false);
    }
    for(int i = 0; i < GROUPSNUM; i++){  //4
        emit updateGroupState(i, false);
    }
}



void L2parser::emitKeySetup(int key_index){
    //qDebug("void L2parser::emitKeySetup(int key_index = %d)", key_index);
    if(!isValidL2W()) return;
    if(key_index<KEYNUM){
        KeyCondition * cond = getCurrentL2W()->getCurrentSettings()->condition[key_index];
        emit setup_key(
                         key_index, //key index
                        cond->getState(),
                        cond->getKeyCode(),
                         cond->getConditionF(idPause),
                         cond->getCooldown4Dongle(),
                         cond->getConditionF(idCondition),
                        cond->getConditionB(idCtrl), // ctrl,
                        cond->getConditionB(idShift) //shift
        );
        getCurrentL2W()->updateRule(key_index);
    }
}

void L2parser::send_all_keys_to_dongle(){
    for(int key_index = 0; key_index<KEYNUM; key_index++){
        emitKeySetup(key_index);
    }
}


void L2parser::toggleRuleState(int key_index){
    if(!isValidL2W()) return;

    if(key_index < KEYNUM ) {
        getCurrentL2W()->getCurrentSettings()->condition[key_index]->setState(!getCurrentL2W()->getCurrentSettings()->condition[key_index]->getState());
        emitKeySetup(key_index);
    }

}

void L2parser::editRule(int key_index){
    //qDebug() << "L2parser::editRule(int key_index) " << key_index;
    if(!isValidL2W()) return;

    if(key_index<KEYNUM){
        KeyCondition cond(".");
        cond = *(getCurrentL2W()->getCurrentSettings()->condition[key_index]);
        KeySettingsDialog dlg(&cond, getCurrentL2W(), key_index);
        if(dlg.exec() == QDialog::Accepted){
           *(getCurrentL2W()->getCurrentSettings()->condition[key_index]) = cond;
            emitKeySetup(key_index);
        }
    }

}

void L2parser::changeNic(const QString &text){
    //qDebug()<< "L2parser::changeNic(const QString &text) " << text;
    if(!isValidL2W()) return;
    getCurrentL2W()->getCurrentSettings()->nic = text;
    doUpdateConditiosList();
}

void L2parser::resetBars(){
    if(!isValidL2W()) return;
    getCurrentL2W()->resetBars();
    getCurrentL2W()->resetSkillbar();
}

void L2parser::doUpdateConditiosList(){
    qDebug("void L2parser::doUpdateConditiosList()");

    if(!isValidL2W())return;
    QVector <QString> list;
    for(int i = 0 ; i < getCurrentL2W()->cond_set_list.count(); i++){
        //qDebug() << " list.append(getCurrentL2W()->cond_set_list[i]->nic) " << getCurrentL2W()->cond_set_list[i]->nic;
        list.append(getCurrentL2W()->cond_set_list[i]->nic);
    }
    emit updateConditiosList(list, getCurrentL2W()->activeCondSet, getCurrentL2W()->getCurrentSettings()->settings_file_name);

}

void L2parser::doUpdateL2WindowsList(){
    //qDebug("void L2parser::doUpdateL2WindowsList()");
    if(!isValidL2W())return;
    QVector <QString> list;

    for(int i = 0 ; i < l2list.count(); i++){
        list.append(l2list[i]->getTitle());
    }
    emit updateL2WindowsList(list, currentl2i, getCurrentL2W()->project_file_name);
    doUpdateConditiosList();

}

void L2parser::setActiveL2Index(int l2_index){
    qDebug("void L2parser::setActiveL2Index(int l2_index = %d)", l2_index);
    if((l2list.isEmpty() || l2list.count() <= currentl2i) && currentl2i != -1) return;
    currentl2i = l2_index;
    emit setActiveL2W(getCurrentL2W(), currentl2i);
    doUpdateL2WindowsList();
}


void L2parser::setActiveCondIndex(int index){
    qDebug("void L2parser::setActiveL2Index(int index = %d)", index);
    if(!isValidL2W())return;
    if(!getCurrentL2W()->isValidIndex(index)) return;
    qDebug("getCurrentL2W()->activateSettings(index = %d) ", index);
    getCurrentL2W()->activateSettings(index);
    doUpdateConditiosList();
}

L2Window * L2parser::getCurrentL2W(){
    if(!isValidL2W())return NULL;
    return l2list[currentl2i];
}

bool L2parser::isValidL2W(){
    if(l2list.isEmpty() || l2list.count() <= currentl2i || currentl2i < 0) return false;
    return true;
}

void L2parser::loadProject(QString file_name){
    //qDebug("void L2parser::loadProject(QString file_name)");
    if(!isValidL2W())return;
    if(file_name.isEmpty() || file_name.isNull()) return;
    getCurrentL2W()->LoadProject(file_name);
    doUpdateL2WindowsList();
    doUpdateConditiosList();
    send_all_keys_to_dongle();
}

void L2parser::saveProject(QString file_name){
    //qDebug("void L2parser::saveProject(QString file_name)");
    if(!isValidL2W())return;
    if(file_name.isEmpty() || file_name.isNull()) return;
    //qDebug("filename: %s", file_name.toStdString().c_str());
    getCurrentL2W()->SaveProject(file_name);
}


void L2parser::saveConfig(QString file_name){
    //qDebug("void L2parser::saveConfig(QString file_name)");
    if(!isValidL2W())return;
    if(file_name.isEmpty() || file_name.isNull()) return;
    //qDebug("filename: %s", file_name.toStdString().c_str());
    getCurrentL2W()->SaveConfig(file_name);
    return;
}

void L2parser::loadConfig(QString file_name){
    //qDebug("void L2parser::loadConfig(QString file_name)");
    if(!isValidL2W())return;
    if(file_name.isEmpty() || file_name.isNull()) return;
    getCurrentL2W()->LoadConfig(file_name);
    doUpdateConditiosList();
    send_all_keys_to_dongle();
}

void L2parser::addConfig(QString file_name){
    //qDebug("void L2parser::addConfig(QString file_name)");
    if(!isValidL2W())return;
    if(file_name.isEmpty() || file_name.isNull()) return;
    getCurrentL2W()->AddConfig(file_name);
    doUpdateConditiosList();
    send_all_keys_to_dongle();
}


void L2parser::resetL2Windows(QVector <HWND> *hwnd_list){
    //qDebug("void L2parser::resetL2Windows(QVector <HWND> *hwnd_list)");
    QVector <L2Window*> l2temp;
    QVector <L2Window*> l2old;
    //qDebug("copy  l2old = l2list");
    l2old = l2list;
    //qDebug("sizes  l2old=%d l2list=%d", l2old.size(), l2list.size());

    L2Window* active = getCurrentL2W();
    //qDebug("L2Window* active = currentl2w() %d", (int)active);

    //qDebug("Start regrouping");

    //Add all  windows
    for(int hwndindex = 0; hwndindex < hwnd_list->size(); hwndindex++){
        bool found = false;
        //Try to find existing windows
        for(int l2index = 0; l2index<l2old.size(); l2index++){
            if(l2old[l2index]->getHWND() == (*hwnd_list)[hwndindex]){
                //qDebug("Found hwnd %d", (int)(*hwnd_list)[hwndindex]);
                l2temp.append(l2old[l2index]);
                //qDebug("l2temp.append size %d", l2temp.size());
                l2old.remove(l2index);
                //qDebug("l2old.remove size %d", l2old.size());
                found = true;
                break;
            }
        }
        // Add new windows
        if(!found){
            //qDebug("Not found hwnd %d", (int)(*hwnd_list)[hwndindex]);
            L2Window* l2w = new L2Window((*hwnd_list)[hwndindex]);
            if(l2w != NULL){
                l2temp.append(l2w);
                l2w->LoadProject(default_file_name);
                //qDebug("l2temp.append size %d", l2temp.size());
            } else {
                qWarning("l2w creation failed");
            }
        }
    }

    // Copy new list to permanent location and set new active window
    int newIndex = 0;
    for(int l2index = 0; l2index<l2temp.size(); l2index++){
        if(l2temp[l2index] == active){
            newIndex = l2index;
            //qDebug("set activeIndex %d", newIndex);
            break;
        }
    }
    l2list.clear();
    if(l2temp.size()>0){
        l2list = l2temp;
        setActiveL2Index(newIndex);
    } else {
        setActiveL2Index(-1);
    }

    //Remove all closed windows
    //qDebug("need to remove %d", l2old.size());
    for(int l2index = 0; l2index<l2old.size(); l2index++){
        delete l2old[l2index];
        //qDebug("delete %d", l2index);
        l2old.remove(l2index);
        //qDebug("remove %d", l2index);
    }

    //qDebug("resetL2Windows done, l2list size %d",  l2list.size());
}

void L2parser::showDongleStatus(unsigned char d_stt,  int updatetime)
{
    Q_UNUSED(updatetime);
    bDongleIsWorking = ((d_stt & (1 << DEVICE_STATUS)) > 0);

}
