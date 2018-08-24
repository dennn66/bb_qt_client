#include "l2window.h"


L2Window::L2Window(HWND winhwnd, QObject *parent) : QObject(parent)
{

    //LOAD CONFIG BB.ini
    //QSettings sett("bb.ini", QSettings::IniFormat);

    l2w = new L2::Window(  winhwnd, parent);
    groupmanager = new GroupManager;
    condmgr = new ConditionMgr;

    //KeyConditionsSet* conditionSet = new KeyConditionsSet();
    //cond_set_list.append(conditionSet);
    //project_file_name = "default.bbproj";
    //activeCondSet = 0;

}



bool L2Window::isSkillRdy(int num){
    if(!l2w->isActive()) return false;
    return isSkillConditionRdy(num);
}

bool L2Window::isSkillConditionRdy(int num){
    KeyCondition* cond = this->getCondMgr()->getCurrentSettings()->condition[num];
    if(!cond->getState()) return false;

    if(getCondMgr()->getConditionSkill(num)){
        if(!l2w->skillbar->isSkillReady(num)){
            return false;
        } else {
            if((l2w->skillbar->whenUsed(num).msecsTo(QDateTime::currentDateTime())) < (cond->getConditionF(idCoolDown)*1000)) return false;
        }
    }

    if(!cond->checkTargetCondition(this->getL2W()->getTargetType())){return false;}// Target Condition
    if(!cond->checkTokenCondition(this->getL2W()->getTokenState())){return false;}// Star Condition

    for(int barnum = idCP; barnum <= idVP; barnum++){
        if(!cond->checkBarCondition(barnum, l2w->getXP(barnum) )) return false;
    }
    if(getL2W()->getTargetType() == TARGETMOB){
        if(!cond->checkBarCondition(idMobHP, l2w->getXP(idMobHP))) return false;
    }
    if(l2w->bEnablePet){
        for(int barnum = idPet1HP; barnum < idPartyHP; barnum++){
            if(!cond->checkBarCondition(barnum, l2w->getXP(barnum) )) return false;
        }
    }
    /*
    if(l2w->bEnableParty && cond->isBarConditionEnabled(idMinMinMemberHP)){
        bool members_cond = false;
        members_cond = cond->checkBarCondition(idMinMinMemberHP, l2w->getXP(idHP), false );
        for(int barnum = idPartyHP; barnum < idPartyHP+l2w->partybox->getMembersNum(); barnum++){
            members_cond = members_cond || cond->checkBarCondition(idMinMinMemberHP, l2w->getXP(barnum), false  );
            if(members_cond) break;
        }
        if(!members_cond) return false ;
    }
    */
    if(l2w->bEnableParty){
        int rulesIDs[] = {idMinMinMemberHP, idMinMinMemberCP, idMinMinMemberMP};
        int condIDs[] = {idPartyHP, idPartyCP, idPartyMP};
        int xpIDs[] = {idHP, idCP, idMP};
        bool members_cond = false;

        for(int i = 0; i<3; i++){
            if(cond->isBarConditionEnabled(rulesIDs[i])){
                members_cond = cond->checkBarCondition(rulesIDs[i], l2w->getXP(xpIDs[i]), false );
                for(int barnum = condIDs[i]; barnum < condIDs[i]+l2w->partybox->getMembersNum(); barnum++){
                    members_cond = members_cond || cond->checkBarCondition(rulesIDs[i], l2w->getXP(barnum), false  );
                    if(members_cond) break;
                }
                if(!members_cond) return false ;
            }
        }
    }


    if(cond->getConditionB(idCheckPet)) if (cond->getConditionB(idPetState) != l2w->bPet) return false;
    if(l2w->bSearchTarget && cond->getConditionB(idCheckRange)&&(getL2W()->getTargetType() == TARGETMOB || getL2W()->getTargetType() == TARGETCHAR)) if (cond->getConditionB(idInRange) != l2w->bInRange) return false;

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
            if(getCondMgr()->getConditionSkill(n) && getCondMgr()->getConditionState(n)){
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
    condmgr->activateSettings(index);
    for(int key_index = 0; key_index < KEYNUM; key_index++){
        updateRule(key_index);
    }
    return true;
}

bool L2Window::updateRule(int key_index){
    if(key_index >= KEYNUM) return false;
    for(int groupcondnum = 0; groupcondnum < GROUPSNUM; groupcondnum++){
        groupmanager->setGroupCondition(key_index, groupcondnum,  getCondMgr()->getCurrentSettings()->condition[key_index]->getGroupState(groupcondnum));
    }

    return true;
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
           if(getCondMgr()->getConditionState(i)){
               if(getGroupManager()->get_visual_skill_group_state(i)){
                   if(getGroupManager()->get_visual_skill_state(i)) {
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



