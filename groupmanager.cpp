#include "groupmanager.h"

GroupManager::GroupManager(QObject *parent) : QObject(parent)
{
    for(int i = 0; i < KEYNUM; i++) {
        l2_skill_state[i] = false;
        old_visual_skill_state[i] = true;
        old_dongle_skill_state[i] = true;
        visual_skill_state[i] = false;
        dongle_skill_state[i] = false;
    }
    for(int i = 0; i < GROUPSNUM; i++) {
        group_state[i] = false;
        old_group_state[i] = true;
    }
    isActiveWindow = false;
}
