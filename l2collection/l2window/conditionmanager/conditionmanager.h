#ifndef L2PROCESSOR_H
#define L2PROCESSOR_H
#include <QObject>
#include <QString>
#include <QtWinExtras/QtWin>
#include <QImage>
#include <QIcon>
#include <QFile>
#include <QTextCodec>
#include <QTextStream>
#include <QSettings>
#include <QMessageBox>

//#include <QWidget>
#include <QtGui>
//#include <QVBoxLayout>

#include <windows.h>

#include "l2collection\l2window\conditionmanager\keycondition.h"
#include "l2collection\l2window\conditionmanager\keyconditionsset.h"


#define L2_OFF false
#define L2_ON true



class ConditionManager  : public QObject
{
    Q_OBJECT
public:

    ConditionManager(QObject *parent = 0);
    QString getTitle();
    QString getNic(){return getCurrentSettings()->nic; }
    QString getNic(int index){return getSettings(index)->nic; }

    QString project_file_name;

    bool LoadProject(QString file_name);
    bool saveProject(QString file_name);
    bool loadConfig(QString file_name);
    bool saveConfig(QString file_name);
    bool AddConfig(QString file_name);
    QString getConditionLabel(int index){if(getCurrentSettings() == NULL) return ""; return getCurrentSettings()->condition[index]->getKeyString();}
    bool getConditionState(int index){if(getCurrentSettings() == NULL) return false; return getCurrentSettings()->condition[index]->getState();}

    bool isValidIndex(int index){
        if((index == -1)||(cond_set_list.isEmpty())||(index >= cond_set_list.size()))return false;
        return true;
    }

    bool activateSettings(int index);
    KeyConditionsSet* getCurrentSettings();
    KeyConditionsSet* getSettings(int index);
    QString getCurrentSettings_file_name();

    bool getConditionSkill(int index){return getCurrentSettings()->condition[index]->getConditionB(idCheckSkillTimeout);}


    int activeCondSet;
    QVector <KeyConditionsSet*> cond_set_list;

private:
signals:

public slots:
};

#endif // L2PROCESSOR_H
