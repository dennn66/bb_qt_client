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

#include "groupmanager.h"
#include "conditionmgr.h"

#include "window.h"
#include "elements/skillbar.h"




class L2Window  : public QObject
{
    Q_OBJECT
public:

    L2Window(HWND winhwnd, QObject *parent = 0);


//    QString getNic(){return getCurrentSettings()->nic; }
//    QString project_file_name;

    //void setGroupState(int num, bool state){groupmanager->setGroupState(num, state);}

    //int LoadProject(QString file_name);
    //int SaveProject(QString file_name);
    //int LoadConfig(QString file_name);
    //int SaveConfig(QString file_name);
    //int AddConfig(QString file_name);
    //QString getConditionLabel(int index){return getCurrentSettings()->condition[index]->getKeyString();}

    //bool getConditionState(int index){return getCurrentSettings()->condition[index]->getState();}

    //bool isValidIndex(int index){
    //    if((index == -1)||(cond_set_list.isEmpty())||(index >= cond_set_list.size()))return false;
    //    return true;
    //}
    bool activateSettings(int index);
    bool updateRule(int key_index);
    //KeyConditionsSet* getCurrentSettings();
    bool isSkillConditionRdy(int num);
    bool isSkillRdy(int num);
    int check();
    QImage* getStatusBk(bool donglestate);


    //bool is_dongle_skill_state_changed  (int i){return groupmanager->is_dongle_skill_state_changed(i);    }
    //bool get_dongle_skill_state         (int i){return groupmanager->get_dongle_skill_state( i);          }
    //bool is_visual_skill_state_changed  (int i){return groupmanager->is_visual_skill_state_changed( i);   }
    //bool get_visual_skill_state         (int i){return groupmanager->get_visual_skill_state( i);          }
    //bool get_visual_skill_group_state   (int i){return groupmanager->get_visual_skill_group_state( i);    }
    //bool is_group_state_changed         (int i){return groupmanager->is_group_state_changed( i);          }
   // bool get_group_state                (int i){return groupmanager->get_group_state( i);                 }

    //void setSkillImg(int n)             {l2w->setSkillImg(n);                                             }

//    bool getConditionSkill(int index){return getCurrentSettings()->condition[index]->getConditionB(idCheckSkillTimeout);}

    //int getTargetType()                 {return l2w->getTargetType();                                     }
    //QColor* getTokenColor()             {return l2w->getTokenColor();                                     }
    //int getTokenState()                 {return l2w->getTokenState();                                     }
    //QIcon* getIcon()                    {return l2w->getIcon();                                           }
    //L2::Skillbar* getSkillbar()         {return l2w->getSkillbar();                                       }

    //QString getTitle()                  {return l2w->getTitle();                                          }

    //void resetBars()                    {l2w->resetBars();                                                }
    //void resetSkillbar()                {l2w->resetSkillbar();                                            }
    //HWND getHWND()                      {return l2w->getHWND();                                           }
    //bool isActiveWindow()               {return l2w->isActive();                                          }
    //void getStatusBtn(QImage* imgStatus, bool pressed) {l2w->getStatusBtn(imgStatus, pressed);            }
    //QRect getL2WRect()                  {return l2w->getL2WRect();                                        }
    //QImage* getImage()                  {return l2w->getImage();                                          }
    //int getXP(int bar)                  {return l2w->getXP(bar);                                          }
    //bool is_res_avialable               (     ){return l2w->is_res_avialable();                           }
    //bool is_res_in_focus                (     ){return l2w->is_res_in_focus();                            }
    //void setMouseCoord(int x, int y)    {l2w->setMouseCoord( x,  y);                                      }
    //qint8  get_res_DeltaX               (     ){return l2w->get_res_DeltaX();                             }
    //qint8  get_res_DeltaY               (     ){return l2w->get_res_DeltaY();                             }



//    int activeCondSet;
//    QVector <KeyConditionsSet*> cond_set_list;

    GroupManager* getGroupManager(){return groupmanager;}
    L2::Window* getL2W(){return l2w;}
    ConditionMgr* getCondMgr(){return condmgr;}

private:
    GroupManager* groupmanager;
    L2::Window* l2w;
    ConditionMgr* condmgr;

//    bool bEnableTimingDebug;



signals:

public slots:
};

#endif // L2WINDOW_H
