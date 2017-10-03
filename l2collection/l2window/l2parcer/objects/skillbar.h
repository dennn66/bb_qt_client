#ifndef SKILLBAR_H
#define SKILLBAR_H

#include <QObject>
#include "processor\keyconditionsset.h"
#include "box.h"
#include "skill.h"

#define TOOLLOFFSET 38
#define TOOLTOFFSET 9
#define TOOLVGAP 14
#define TOOLHGAP 5
#define TOOLH2GAP 7



class Skillbar : public Box
{

public:
    Skillbar(Box *parent = 0);
    QImage* getSkillImg(int n){return skill[n].getSkillImg();}
    QImage* setSkillImg(int n, QImage* image);

    //void checkSkillbar(QImage image);
    void checkSkill(int n, QImage* image);
    bool state(){return getLeftStatus();}
    bool isSkillReady(int num){return skill[num].isSkillReady();}
    QDateTime whenUsed(int num){return skill[num].whenUsed();}
    bool isInReuse(int num){return skill[num].isInReuse();}
    QDateTime lastReady(int num){return skill[num].lastReady();}

    void initSkillbarFrame();
    void initSkillbarImage(QImage* image);
    void drawStatus(QImage* imgStatus, QRect r);


private:
    //QImage parceSkillbar(int n, QImage* image);
    Skill skill[KEYNUM];
};

#endif // SKILLBAR_H
