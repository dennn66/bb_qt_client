#include "l2window.h"


L2Window::L2Window(HWND winhwnd, QObject *parent) : QObject(parent)
{

    //LOAD CONFIG BB.ini
    QSettings sett("bb.ini", QSettings::IniFormat);

    l2w = new L2::Window(  winhwnd, parent);

    groupmanager = new GroupManager;


    KeyConditionsSet* conditionSet = new KeyConditionsSet();
    cond_set_list.append(conditionSet);
    project_file_name = "default.bbproj";
    activeCondSet = 0;

}



bool L2Window::isSkillRdy(int num){
    if(!l2w->isActive()) return false;
    return isSkillConditionRdy(num);
}

bool L2Window::isSkillConditionRdy(int num){
    KeyCondition* cond = this->getCurrentSettings()->condition[num];
    if(!cond->getState()) return false;

    if(getConditionSkill(num)){
        if(!l2w->skillbar->isSkillReady(num)){
            return false;
        } else {
            if((l2w->skillbar->whenUsed(num).msecsTo(QDateTime::currentDateTime())) < (cond->getConditionF(idCoolDown)*1000)) return false;
        }
    }

    if(!cond->checkTargetCondition(this->getTargetType())){return false;}// Target Condition
    if(!cond->checkTokenCondition(this->getTokenState())){return false;}// Star Condition

    for(int barnum = idCP; barnum <= idVP; barnum++){
        if(!cond->checkBarCondition(barnum, l2w->getXP(barnum) )) return false;
    }
    if(getTargetType() == TARGETMOB){
        if(!cond->checkBarCondition(idMobHP, l2w->getXP(idMobHP))) return false;
    }
    if(l2w->bEnablePet){
        for(int barnum = idPet1HP; barnum < idPartyHP; barnum++){
            if(!cond->checkBarCondition(barnum, l2w->getXP(barnum) )) return false;
        }
    }
    if(l2w->bEnableParty && cond->isBarConditionEnabled(idMinMemberHP)){
        bool members_cond = false;
        members_cond = cond->checkBarCondition(idMinMemberHP, l2w->getXP(idHP), false );
        for(int barnum = idPartyHP; barnum < idPartyHP+l2w->partybox->getMembersNum(); barnum++){
            members_cond = members_cond || cond->checkBarCondition(idMinMemberHP, l2w->getXP(barnum), false  );
            if(members_cond) break;
        }
        if(!members_cond) return false ;
    }
    if(cond->getConditionB(idCheckPet)) if (cond->getConditionB(idPetState) != l2w->bPet) return false;
    if(l2w->bSearchTarget && cond->getConditionB(idCheckRange)&&(getTargetType() == TARGETMOB || getTargetType() == TARGETCHAR)) if (cond->getConditionB(idInRange) != l2w->bInRange) return false;

    int parentskill = cond->getConditionI(idPauseSkillNum);
    if(parentskill < 0xFF){
        if(l2w->skillbar->isSkillReady(parentskill)) return false;
        if((l2w->skillbar->lastReady(parentskill).msecsTo(QDateTime::currentDateTime())) < (cond->getConditionF(idSkillPause)*1000)) return false;
    }
    return true;
}




int L2Window::check(){
    l2w->check();
    groupmanager->isL2Active(l2w->isActive());


    if(l2w->getSkillbar()->getState()) {
        for(int n=0;n<KEYNUM;n++){
            if(getConditionSkill(n) && getConditionState(n)){
                l2w->checkSkill(n);
            }
        }
    }

    for(int i = 0; i < KEYNUM; i++){  //48 keys
        groupmanager->set_l2_skill_state(i, isSkillConditionRdy(i));
    }

    return 1;
}





bool L2Window::activateSettings(int index){
    if(!isValidIndex(index))return false;
    activeCondSet = index;
    for(int key_index = 0; key_index < KEYNUM; key_index++){
        updateRule(key_index);
    }
    return true;
}

bool L2Window::updateRule(int key_index){
    if(key_index >= KEYNUM) return false;
    for(int groupcondnum = 0; groupcondnum < GROUPSNUM; groupcondnum++){
        groupmanager->setGroupCondition(key_index, groupcondnum,  getCurrentSettings()->condition[key_index]->getGroupState(groupcondnum));
    }

    return true;
}


KeyConditionsSet* L2Window::getCurrentSettings(){
    if(!isValidIndex(activeCondSet))return NULL;
    return cond_set_list[activeCondSet];
}

int L2Window::LoadProject(QString file_name){
    //qDebug("L2Window::LoadProject");
    QSettings sett(file_name, QSettings::IniFormat);

    project_file_name = file_name;
    //qDebug("Proj File: %s", file_name.toStdString().c_str());
    int proj_size = sett.value("MAIN/count", 0).toInt();
    //qDebug("count: %d", proj_size);
    if(proj_size < 1) return 1;

    QString topic = "SETTINGS";

    for(int i = 0;i < proj_size;i++){
        QString var;
        QTextStream varstream(&var);
        var = topic;
        varstream  << "/settings_file_name" <<  i+1;
        //qDebug("Topic: %s", var.toStdString().c_str());

        QString settings_file_name = sett.value(var.toStdString().c_str(), "default.cfg").toString();
        //qDebug("Settings File: %s", settings_file_name.toStdString().c_str());

        if(i < cond_set_list.size()){
            activeCondSet = i;
            LoadConfig(settings_file_name);
            //qDebug("Load: %d", i);

        } else {
            AddConfig(settings_file_name);
            //qDebug("Add: %d", i);
        }
        activateSettings(0);
    }
    while(proj_size < cond_set_list.size()){
        KeyConditionsSet* tmp = cond_set_list.last();
        //qDebug("Removed node %s", tmp->settings_file_name.toStdString().c_str());
        cond_set_list.removeLast();
        delete(tmp);
        //qDebug("Removed node %s", tmp->settings_file_name.toStdString().c_str());
    }
    //qDebug("FIN L2Window::LoadProject");

    return 1;
}

int L2Window::SaveProject(QString file_name){

    //qDebug("L2Window::SaveProject");

    QSettings sett(file_name, QSettings::IniFormat);

    project_file_name = file_name;
    //qDebug("File: %s", file_name.toStdString().c_str());
    sett.setValue("MAIN/count", cond_set_list.size());

    QString topic = "SETTINGS";

    for(int i = 0;i < cond_set_list.size();i++){
        QString var;
        QTextStream varstream(&var);
        var = topic;
        varstream  << "/settings_file_name" <<  i+1;
        sett.setValue(var.toStdString().c_str(), cond_set_list[i]->settings_file_name);
    }
    return 1;
}

int L2Window::AddConfig(QString file_name){
    //qDebug("L2Window::LoadConfig");
    KeyConditionsSet* conditionSet = new KeyConditionsSet();
    cond_set_list.append(conditionSet);
    activeCondSet = cond_set_list.size() - 1;
    getCurrentSettings()->LoadConfig(file_name);
    activateSettings(activeCondSet);
    return 1;
}

int L2Window::LoadConfig(QString file_name){
    //qDebug("L2Window::LoadConfig");
    getCurrentSettings()->LoadConfig(file_name);
    //qDebug("FIN L2Window::LoadConfig");

    return 1;
}

int L2Window::SaveConfig(QString file_name){

    //qDebug("L2Window::LoadConfig");
    getCurrentSettings()->SaveConfig(file_name);
    return 1;
}



QImage* L2Window::getStatusBk(bool donglestate){

    QRect l2rect = l2w->getL2WRect();

    QImage* imgStatus = new QImage(QSize(l2rect.width(),l2rect.height()), QImage::Format_ARGB32);
    if(imgStatus->isNull()){
        return imgStatus;
    }

  // QPainter p;
    QPainter p;
   p.begin(imgStatus);
       QPen skillpen;
       skillpen.setWidth(3);
       if(donglestate){
           skillpen.setColor(QColor("#FF00FF00"));
       } else {
           skillpen.setColor(QColor("#00000000"));
       }
       p.setPen(skillpen);
       p.setBrush(QBrush(QColor("#00FFFFFF"), Qt::SolidPattern));
       QRect r = imgStatus->rect();
       QRect mainframe(r.x(), r.y(), r.width()-3, r.height()-3);

       p.drawRect(mainframe);

       skillpen.setWidth(2);
       for(int i = 0; i < KEYNUM; i++){  //48 keys
           if(getConditionState(i)){
               if(get_visual_skill_group_state(i)){
                   if(get_visual_skill_state(i)) {
                       skillpen.setColor(QColor("#FF00FF00"));
                   } else {
                       skillpen.setColor(QColor("#FFFFFFFF"));
                       skillpen.setWidth(3);
                   }
               } else {
                   skillpen.setColor(QColor("#88FF0000"));
               }
           } else {
               skillpen.setColor(QColor("#00FFFFFF"));
           }
           p.setPen(skillpen);
           p.drawRect(l2w->getSkillRect(i));
       }

       l2w->drawOverlayedStatus(&p, &skillpen);

   p.end();
   return imgStatus;
}


