#include "skillstate.h"
using namespace L2;
SkillState::SkillState(int id)
{
    stateid = id;
    for(int i = 0; i < TIME_NUM; i++) time[i] = QDateTime::currentDateTime();
}
