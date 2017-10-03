#ifndef L2Processor_H
#define L2Processor_H
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

#include "processor\keycondition.h"
#include "processor\keyconditionsset.h"
#include "l2collection\L2Processor\l2parser\objects\skillbar.h"
#include "l2collection\L2Processor\l2parser\objects\mainbox.h"
#include "l2collection\L2Processor\l2parser\objects\mobbox.h"
#include "l2collection\L2Processor\l2parser\objects\petbox.h"


#define L2_OFF false
#define L2_ON true



class L2Processor  : public QObject
{
    Q_OBJECT
public:

    L2Processor(HWND winhwnd, QObject *parent = 0);
    bool init(QImage image);
    QIcon* getIcon(){return L2icon; }
    int getTokenState(){return mobbarbox->getTokenState();}
    QColor* getTokenColor(){return mobbarbox->getTokenColor(); }
    QString getTitle();
    QString getNic(){return getCurrentSettings()->nic; }
    HWND getHWND(){return hwnd; }
    QString project_file_name;
    QPoint windowtopright;
    bool isActiveWindow;

    int LoadProject(QString file_name);
    int SaveProject(QString file_name);
    int LoadConfig(QString file_name);
    int SaveConfig(QString file_name);
    int AddConfig(QString file_name);
    int getXP(int bar);
    int check();
    QString getConditionLabel(int index){return getCurrentSettings()->condition[index]->getKeyString();}

    bool getConditionState(int index){return getCurrentSettings()->condition[index]->getState();}

    void resetBars();
    void resetSkillbar(){skillsread = false;}
    bool isValidIndex(int index){
        if((index == -1)||(cond_set_list.isEmpty())||(index >= cond_set_list.size()))return false;
        return true;
    }
    bool activateSettings(int index);
    KeyConditionsSet* getCurrentSettings();
    bool isSkillConditionRdy(int num);
    bool isSkillRdy(int num);

    bool getConditionSkill(int index){return getCurrentSettings()->condition[index]->getConditionB(idCheckSkillTimeout);}

    Skillbar* getSkillbar(){return skillbar;}
    Barbox* getMobbarbox(){return mobbarbox;}
    Barbox* getMainbarbox(){return mainbarbox;}
    QImage* setSkillImg(int n){return skillbar->setSkillImg(n, &image);}

    bool getPetStatus(){return petbarbox->getLeftStatus();}
    void getBarStatistic();
    int getTargetType(){return mobbarbox->getTargetType();}
    void getStatusBtn(QImage* imgStatus, bool pressed);


    int activeCondSet;
    QVector <KeyConditionsSet*> cond_set_list;

    Skillbar* skillbar;
private:

    QImage image;


    HWND hwnd;
    bool status;
    QIcon* L2icon;
    int image_width;
    int image_height;
    bool skillsread;

    Mainbox* mainbarbox;
    Mobbox* mobbarbox;
    Petbox* petbarbox;

    bool bPet;
    bool bEnablePet;
//    bool bEnableTimingDebug;


    void capture();
    bool findCPHPMP(QImage image);

signals:

public slots:
};

#endif // L2Processor_H
