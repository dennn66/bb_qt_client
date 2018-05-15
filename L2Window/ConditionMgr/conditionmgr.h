#ifndef CONDITIONMGR_H
#define CONDITIONMGR_H

#include <QObject>
#include "keycondition.h"
#include "keyconditionsset.h"


class ConditionMgr : public QObject
{
    Q_OBJECT

public:
    ConditionMgr(QObject *parent = 0);
    QString getNic(){return getCurrentSettings()->nic; }

    int LoadProject(QString file_name);
    int SaveProject(QString file_name);
    int LoadConfig(QString file_name);
    int SaveConfig(QString file_name);
    int AddConfig(QString file_name);
    QString getConditionLabel(int index){return getCurrentSettings()->condition[index]->getKeyString();}

    bool getConditionState(int index){return getCurrentSettings()->condition[index]->getState();}

    bool isValidIndex(int index){
        if((index == -1)||(cond_set_list.isEmpty())||(index >= cond_set_list.size()))return false;
        return true;
    }
    bool activateSettings(int index);
    bool updateRule(int key_index);
    KeyConditionsSet* getCurrentSettings();

    bool getConditionSkill(int index){return getCurrentSettings()->condition[index]->getConditionB(idCheckSkillTimeout);}



    QString project_file_name;
    int activeCondSet;
    QVector <KeyConditionsSet*> cond_set_list;



};

#endif // CONDITIONMGR_H
