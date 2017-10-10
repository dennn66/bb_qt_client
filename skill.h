#ifndef SKILL_H
#define SKILL_H

#include "l2graphicsobject.h"
#include "skillstate.h"

#define SKILLREUSE 0
#define SKILLRDY 1
#define SKILLOTHER 2

#define TOOLSELL 32

class Skill : public L2GraphicObject
{
public:
    explicit Skill(L2GraphicObject *parent = 0);
    QRect getSkillRect(){ return frame;}
    QImage* getSkillImg(){ return &tool;}
    void setSkillImg(QImage* image){ tool =  image->copy(frame.x(), frame.y(), frame.width(), frame.height());}
    bool isSkillReady(){return skillrdy;}
    void setSkillReady(bool state){skillrdy = state;}
    void setFrame(QRect r){frame = r;}

    void checkSkill(QImage* currentimage);
    QDateTime whenUsed(){return ready->getTime(TIME_LASTEND);}
    QDateTime lastReady(){return ready->getTime(TIME_LASTRDY);}
    bool isInReuse(){return(current == reuse); }

protected:
    bool skillrdy;
    QImage tool;
    QRect frame;
    SkillState* ready;
    SkillState* reuse;
    SkillState* current;
};

#endif // SKILL_H
