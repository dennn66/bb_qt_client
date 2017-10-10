#include "groupcondition.h"

GroupCondition::GroupCondition(QObject *parent) : QObject(parent)
{
    for(int i = 0; i < GROUPSNUM; i++) { group_condition[i] = false;}

}
