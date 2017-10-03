#ifndef SKILLSTATE_H
#define SKILLSTATE_H

#include <QDateTime>

#define TIME_START      0
#define TIME_END        1
#define TIME_LASTSTART  2
#define TIME_LASTEND    3
#define TIME_LASTRDY    4
#define TIME_NUM        5

class SkillState
{
public:
    SkillState(int id);
    int getStateID(){return stateid;}
    void setTime(int id, QDateTime t){time[id] = t;}
    QDateTime getTime(int id){return time[id];}

protected:
    int stateid;
    QDateTime time[TIME_NUM];

};

#endif // SKILLSTATE_H
