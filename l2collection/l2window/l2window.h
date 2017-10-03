#ifndef L2WINDOW_H
#define L2WINDOW_H

#include <QObject>
#include "l2collection\l2window\l2parser\l2parser.h"
#include "l2collection\l2window\conditionmanager\conditionmanager.h"

class L2Window : public QObject
{
    Q_OBJECT
public:
    explicit L2Window(HWND winhwnd, QObject *parent = 0);

    ConditionManager* conditionmanager;
    L2Parser* l2parser;

    QImage* getSkillImg(int skillnum){return l2parser->getSkillbar()->getSkillImg(skillnum);}
    void setSkillImg(int skillnum){l2parser->setSkillImg(skillnum);}
    HWND getHWND(){return  l2parser->getHWND(); }
    bool isActionlRdyToApply(int num);
    bool isActionTurnedOn(int num);
    bool isActionConditionRdy(int num);

private:

signals:

public slots:
};

#endif // L2WINDOW_H
