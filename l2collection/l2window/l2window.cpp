#include "l2window.h"

L2Window::L2Window(HWND winhwnd, QObject *parent) : QObject(parent)
{
    conditionmanager = new ConditionManager(this);
    l2parser = new L2Parser(winhwnd, this);
}

bool L2Window::isActionlRdyToApply(int num){
    if(!l2parser->isActiveWindow()) return false;
    return isActionConditionRdy(num);
}

bool L2Window::isActionTurnedOn(int num){
    return conditionmanager->getConditionState(num);
}


bool L2Window::isActionConditionRdy(int num){
    if(!isActionTurnedOn(num)) return false;
    KeyCondition* cond = conditionmanager->getCurrentSettings()->condition[num];
    if(conditionmanager->getConditionSkill(num)){
        if(!l2parser->skillbar->isSkillReady(num)){
            return false;
        } else {
            if((l2parser->skillbar->whenUsed(num).msecsTo(QDateTime::currentDateTime())) < (cond->getConditionF(idCoolDown)*1000)) return false;
        }
    }



    if(!cond->checkTargetCondition(l2parser->getTargetType())){return false;}// Target Condition
    if(!cond->checkTokenCondition(l2parser->getTokenState())){return false;}// Star Condition


    for(int barnum = idCP; barnum < idVP+1; barnum++){
        if(!cond->checkBarCondition(barnum, l2parser->getXP(barnum) )) return false;
    }
    if(l2parser->getTargetType() == TARGETMOB){
        if(!cond->checkBarCondition(idMobHP, l2parser->getXP(idMobHP))) return false;
    }
    for(int barnum = idPet1HP; barnum < BARNUM; barnum++){
        if(!cond->checkBarCondition(barnum, l2parser->getXP(barnum) )) return false;
    }
    if(cond->getConditionB(idCheckPet)) if (cond->getConditionB(idPetState) != l2parser->bPet) return false;

    int parentskill = cond->getConditionI(idPauseSkillNum);
    if(parentskill < 0xFF){
        //if(!skillbar->isInReuse(parentskill)) return false;
        //if((skillbar->whenUsed(parentskill).msecsTo(QDateTime::currentDateTime())) < (cond->getConditionF(idSkillPause)*1000)) return false;

        if(l2parser->skillbar->isSkillReady(parentskill)) return false;
        if((l2parser->skillbar->lastReady(parentskill).msecsTo(QDateTime::currentDateTime())) < (cond->getConditionF(idSkillPause)*1000)) return false;

    }

    //for(int i = 0; i < idNoUseSkillNum; i++){
    //    if((cond->getConditionB(idNoUseSkillState+i) != false)){
    //        if(skillbar->skillstate(i)) return false;
    //    }
    //}
    return true;
}
