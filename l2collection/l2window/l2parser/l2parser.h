#ifndef L2PARSER_H
#define L2PARSER_H
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

#include "l2collection\l2window\l2parser\objects\skillbar.h"
#include "l2collection\l2window\l2parser\objects\mainbox.h"
#include "l2collection\l2window\l2parser\objects\mobbox.h"
#include "l2collection\l2window\l2parser\objects\petbox.h"


#define L2_OFF false
#define L2_ON true



class L2Parser  : public QObject
{
    Q_OBJECT
public:

    L2Parser(HWND winhwnd, QObject *parent = 0);
    bool init(QImage image);
    QIcon* getIcon(){return L2icon; }
    int getTokenState(){return mobbarbox->getTokenState();}
    QColor* getTokenColor(){return mobbarbox->getTokenColor(); }
    HWND getHWND(){return hwnd; }
    QPoint windowtopright;
    bool isActiveL2Window;
    bool isActiveWindow(){return isActiveL2Window;}
    int getXP(int bar);
    int check();
    void resetBars();
    void resetSkillbar(){skillsread = false;}
    bool isSkillRdy(int num);

    Skillbar* getSkillbar(){return skillbar;}
    Barbox* getMobbarbox(){return mobbarbox;}
    Barbox* getMainbarbox(){return mainbarbox;}
    QImage* setSkillImg(int n){return skillbar->setSkillImg(n, &image);}

    bool getPetStatus(){return petbarbox->getLeftStatus();}
    void getBarStatistic();
    int getTargetType(){return mobbarbox->getTargetType();}
    void getStatusBtn(QImage* imgStatus, bool pressed);
    QString getTitle();

    Skillbar* skillbar;
        bool bPet;
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


    bool bEnablePet;
//    bool bEnableTimingDebug;


    void capture();
    bool findCPHPMP(QImage image);

signals:

public slots:
};

#endif // L2PARSER_H
