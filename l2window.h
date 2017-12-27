#ifndef L2WINDOW_H
#define L2WINDOW_H
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

#include "keycondition.h"
#include "keyconditionsset.h"
#include "skillbar.h"
#include "mainbox.h"
#include "mobbox.h"
#include "petbox.h"
#include "groupmanager.h"


#define L2_OFF false
#define L2_ON true



class L2Window  : public QObject
{
    Q_OBJECT
public:

    L2Window(HWND winhwnd, QObject *parent = 0);
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

    void setGroupState(int num, bool state){groupmanager->setGroupState(num, state);}

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
    bool updateRule(int key_index);
    KeyConditionsSet* getCurrentSettings();
    bool isSkillConditionRdy(int num);
    bool isSkillRdy(int num);

    bool is_dongle_skill_state_changed  (int i){return groupmanager->is_dongle_skill_state_changed(i);    }
    bool get_dongle_skill_state         (int i){return groupmanager->get_dongle_skill_state( i);          }
    bool is_visual_skill_state_changed  (int i){return groupmanager->is_visual_skill_state_changed( i);   }
    bool get_visual_skill_state         (int i){return groupmanager->get_visual_skill_state( i);          }
    bool get_visual_skill_group_state   (int i){return groupmanager->get_visual_skill_group_state( i);    }
    bool is_group_state_changed         (int i){return groupmanager->is_group_state_changed( i);          }
    bool get_group_state                (int i){return groupmanager->get_group_state( i);                 }


    bool getConditionSkill(int index){return getCurrentSettings()->condition[index]->getConditionB(idCheckSkillTimeout);}

    Skillbar* getSkillbar(){return skillbar;}
    Barbox* getMobbarbox(){return mobbarbox;}
    Barbox* getMainbarbox(){return mainbarbox;}
    QImage* setSkillImg(int n){return skillbar->setSkillImg(n, &image);}

    bool getPetStatus(){return petbarbox->getLeftStatus();}
    void getBarStatistic();
    int getTargetType(){return mobbarbox->getTargetType();}
    void getStatusBtn(QImage* imgStatus, bool pressed);
    void getStatusBk(QImage* imgStatus, bool donglestate);
    QRect getL2WRect(){return l2windowrect;}
    QRect getSkillRect(int i){return skillbar->getSkillRect(i);}

    int activeCondSet;
    QVector <KeyConditionsSet*> cond_set_list;

    Skillbar* skillbar;
private:

    QImage image;
    GroupManager* groupmanager;


    HWND hwnd;
    bool status;
    QIcon* L2icon;
    int image_width;
    int image_height;
    QRect l2windowrect;
    bool skillsread;

    Mainbox* mainbarbox;
    Mobbox* mobbarbox;
    Petbox* petbarbox;

    bool bPet;
    bool bEnablePet;
//    bool bEnableTimingDebug;


    void capture();
    bool findCPHPMP(QImage image);
    QRect getBarRect(int index);

signals:

public slots:
};

#endif // L2WINDOW_H
