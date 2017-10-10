#ifndef GROUPMANAGER_H
#define GROUPMANAGER_H

#include <QObject>
#include <windows.h>

#include "groupcondition.h"
#define KEYNUM 48
class GroupManager : public QObject
{
    Q_OBJECT
public:
    explicit GroupManager(QObject *parent = 0);

    bool is_dongle_skill_state_changed  (int i){bool changed = !(dongle_skill_state[i] == old_dongle_skill_state[i]); old_dongle_skill_state[i] = dongle_skill_state[i]; return changed;}
    bool is_visual_skill_state_changed  (int i){bool changed = !(visual_skill_state[i] == old_visual_skill_state[i]); old_visual_skill_state[i] = visual_skill_state[i]; return changed;}
    bool is_group_state_changed         (int i){bool changed = !(group_state[i]        == old_group_state[i]       ); old_group_state[i]        = group_state[i];        return changed;}
    bool get_dongle_skill_state         (int i){return dongle_skill_state[i];}
    bool get_visual_skill_state         (int i){return visual_skill_state[i];}
    bool get_visual_skill_group_state   (int i){return checkGroupRule(i);}

    bool get_group_state                (int i){return group_state[i];       }

    void set_l2_skill_state(int num, bool state){ l2_skill_state[num] = state; updateSkill(num);}
    void setGroupState(int num,  bool state){old_group_state[num] = group_state[num]; group_state[num] = state;}
    void setGroupCondition(int skill_num, int condition_num,  bool state){group_condition[skill_num].setGroupState(condition_num, state);}
    void isL2Active(bool isActive){ isActiveWindow = isActive;}


private:
    bool group_state[GROUPSNUM];
    bool old_group_state[GROUPSNUM];
    bool l2_skill_state[KEYNUM];
    bool visual_skill_state[KEYNUM];
    bool dongle_skill_state[KEYNUM];
    bool old_visual_skill_state[KEYNUM];
    bool old_dongle_skill_state[KEYNUM];
    GroupCondition group_condition[KEYNUM];
    bool isActiveWindow;


    bool checkGroupRule(int num){
        bool result = false;
        for(int i = 0; i < GROUPSNUM ; i++){
            result = result || (group_condition[num].getGroupState(i) && group_state[i]);
        }
        return result;
    }
    void updateSkills(){ for(int i = 0; i < KEYNUM; i++)updateSkill(i);}
    void updateSkill(int num){
        visual_skill_state[num] = l2_skill_state[num] && checkGroupRule(num);
        dongle_skill_state[num] = visual_skill_state[num] && isActiveWindow;

    }


public slots:


};

#endif // GROUPMANAGER_H
