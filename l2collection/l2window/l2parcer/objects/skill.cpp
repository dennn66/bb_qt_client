#include "skill.h"

Skill::Skill(L2GraphicObject *parent) : L2GraphicObject(parent)
{
    skillrdy = false;
    ready = new SkillState(SKILLRDY);
    reuse = new SkillState(SKILLREUSE);
    current = ready;
}

void Skill::checkSkill(QImage* image){
    bool topleft_is_not_the_same=false;
    bool topright_is_not_the_same=false;

    QImage currentimage =  image->copy(frame.x(), frame.y(), frame.width(), frame.height());

    for(int i=TOOLSELL/2-2; i<TOOLSELL/2; i ++)
    {
        if(topleft_is_not_the_same) break;
        for(int j=0; j<2; j ++)
        {
            if(QColor(getSkillImg()->pixel(i,j)).name()!=QColor(currentimage.pixel(i,j)).name())
            {
                topleft_is_not_the_same=true;
                break;
            }
        }
    }
    setSkillReady(!topleft_is_not_the_same);

    for(int i=TOOLSELL/2; i<TOOLSELL/2+2; i ++)
    {
        if(topright_is_not_the_same) break;
        for(int j=0; j<2; j ++)
        {
            if(QColor(getSkillImg()->pixel(i,j)).name()!=QColor(currentimage.pixel(i,j)).name())
            {
                topright_is_not_the_same=true;
                break;
            }
        }
    }


    //STATE MACHINE
    int newstate;
    if(isSkillReady()){
        newstate = SKILLRDY;
    } else if(topright_is_not_the_same){
        newstate = SKILLOTHER;
    } else {
        newstate = SKILLREUSE;
    }

    switch (current->getStateID()) {
    case SKILLRDY:
        switch (newstate) {
        case SKILLRDY:
            current->setTime(TIME_END, QDateTime::currentDateTime());
            current->setTime(TIME_LASTRDY, QDateTime::currentDateTime());
            break;
        case SKILLREUSE:
            current->setTime(TIME_LASTEND, current->getTime(TIME_END));
            current = reuse;
            current->setTime(TIME_LASTSTART, current->getTime(TIME_START));
            current->setTime(TIME_START, QDateTime::currentDateTime());


            break;
        default:
            current->setTime(TIME_END, QDateTime::currentDateTime());
            break;
        }
        break;
    case SKILLREUSE:
        switch (newstate) {
        case SKILLRDY:
            current->setTime(TIME_LASTEND, current->getTime(TIME_END));
            current = ready;
            current->setTime(TIME_LASTSTART, current->getTime(TIME_START));
            current->setTime(TIME_START, QDateTime::currentDateTime());
            break;
        case SKILLREUSE:
            current->setTime(TIME_END, QDateTime::currentDateTime());
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}
