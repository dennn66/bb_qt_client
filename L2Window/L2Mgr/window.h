#ifndef WINDOW_H
#define WINDOW_H

#include <QObject>



#include "elements/skillbar.h"
#include "elements/mainbox.h"
#include "elements/mobbox.h"
#include "elements/petbox.h"
#include "elements/partybox.h"
#include "elements/resurectionbox.h"
#include "elements/targetbox.h"


#define L2_OFF false
#define L2_ON true


namespace L2 {

class Window  : public QObject
{
    Q_OBJECT
public:
    Window(HWND winhwnd, QObject *parent = 0);

    void getStatusBtn(QImage* imgStatus, bool pressed);
    void drawOverlayedStatus(QPainter* p, QPen* skillpen);

    void getBarStatistic();
    int getXP(int bar);
    void  check();
    bool init(QImage image);
    QString getTitle();

    void resetBars();
    void resetSkillbar()                {skillsread = false;                                                }
    void checkSkill(int n)                  {skillbar->checkSkill(n, &image);                                   }

    HWND getHWND()                      {return hwnd;                                                       }

    QIcon* getIcon()                    {return L2icon;                                                     }
    int getTokenState()                 {return mobbarbox->getTokenState();                                 }
    QColor* getTokenColor()             {return mobbarbox->getTokenColor();                                 }

    bool is_res_avialable               (     ){return resbox->getOkDetected();                             }
    bool is_res_in_focus                (     ){return resbox->isOkInFocus();                               }
    void setMouseCoord(int x, int y)    {resbox->setMouseCoord(x-l2windowrect.x(), y-l2windowrect.y());     }
    qint8  get_res_DeltaX               (     ){return resbox->getDeltaX();                                 }
    qint8  get_res_DeltaY               (     ){return resbox->getDeltaY();                                 }
    Skillbar* getSkillbar()             {return skillbar;                                                   }
    Barbox* getMobbarbox()              {return mobbarbox;                                                  }
    Barbox* getMainbarbox()             {return mainbarbox;                                                 }
    QImage* setSkillImg(int n)          {return skillbar->setSkillImg(n, &image);                           }

    bool getPetStatus()                 {return petbarbox->getLeftStatus();                                 }
    int getTargetType()                 {return mobbarbox->getTargetType();                                 }
    QRect getL2WRect()                  {return l2windowrect;                                               }
    QRect getSkillRect(int i)           {return skillbar->getSkillRect(i);                                  }
    QImage* getImage()                  {return &image;                                                     }
    bool isActive()                     {return isActiveWindow;                                             }


    Skillbar* skillbar;
    Partybox* partybox;

    bool bPet;
    bool bParty;
    bool bInRange;
    bool bEnablePet;
    bool bEnableParty;
    bool bSearchTarget;
    bool bShowRange;


private:


    bool status;

    HWND hwnd;
    QImage image;


    Mainbox* mainbarbox;
    Mobbox* mobbarbox;
    Petbox* petbarbox;
    ResurrectionBox* resbox;
    TargetBox* targetbox;


    QIcon* L2icon;
    int image_width;
    int image_height;
    QRect l2windowrect;
    bool skillsread;

    QPoint windowtopright;
    bool isActiveWindow;




    void capture();
    bool findCPHPMP(QImage image);
    QRect getBarRect(int index);


};
}
#endif // WINDOW_H
