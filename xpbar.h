#ifndef XPBAR_H
#define XPBAR_H

#include <QtWinExtras/QtWin>
#include <QImage>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QSettings>
#include "l2graphicsobject.h"


#define BAR_OFF false
#define BAR_ON true
#define XP_ERR 128
#define XP_NPC 254
#define XP_CHAR 255



#define CP_COLOR qRgb(136, 90, 0) //$CP_COLOR = 0x805300,+(133,100,32)
#define BK_CP_COLOR qRgb(52,42,14) //$CP_COLOR = 0x805300,+
#define HP_COLOR qRgb(121, 28, 17) //$HP_COLOR = 0x881D18,+
#define BK_HP_COLOR qRgb(56,33,30) //$HP_COLOR = 0x881D18,+
#define MP_COLOR qRgb(5, 57, 134) //$MP_COLOR = 0x03327E,+
#define BK_MP_COLOR qRgb(25,38,60) //$MP_COLOR = 0x03327E,+
#define MobHP_COLOR qRgb(111,23,19) //$MobHP_COLOR = 0x881D18,+
#define BK_MobHP_COLOR qRgb(54,30,28) //$MobHP_COLOR = 0x881D18,+
#define MobMP_COLOR qRgb(5,63,130) //$PET_MP_COLOR = 0x074C9C,+
#define BK_MobMP_COLOR qRgb(26,41,67) //$PET_MP_COLOR = 0x074C9C,+
#define VP_COLOR qRgb(65, 90, 24)
#define BK_VP_COLOR qRgb(32, 37, 25)


class XPBar  : public L2GraphicObject
{
public:
    int yXP;
    int xBegin;
    int xEnd;
    bool bEnableBarSampling;

    explicit XPBar(L2GraphicObject *parent = 0);
    ~XPBar();
    bool getStatus();
    int getXP();
    void setXP(int i){XP = i;}
    int getY();
    int getBegin();
    int getEnd();
    bool findXPBar(QImage* image, RECT targetRect);
    bool checkXPBar(QImage* image);
    int checkXPBarPartial(QImage* image, int begin, int end);
    int checkXPPatternPartial(QImage* image, int pos);
    void setColors(QRgb color, QRgb bk_color);
    void setbarID(int id);
    int getbarID(){return barID;}
    void setStatus(bool stt);
    bool patternMethod;
    QImage barpattern;



protected:
    bool status;
    int barID;
    int XP;
    QRgb barcolor;
    QRgb barbkclr;

signals:

public slots:
};

#endif // XPBAR_H
