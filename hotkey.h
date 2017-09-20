#ifndef HOTKEY_H
#define HOTKEY_H

#include <QObject>
#include <QtWinExtras/QtWin>

#define K_DISABLE 0
#define K_ENABLE 1
#define K_TOGGLE 2
#define K_NUM 3

#define B_ONOFF 0
#define B_B1    1
#define B_B2    2
#define B_B3    3
#define B_B4    4
#define B_NUM   5

#define VK_NUM K_NUM*B_NUM

class HotKey : public QObject
{
    Q_OBJECT
public:
    explicit HotKey(int id, bool modifier, DWORD modifierCode, DWORD activationKeyCode, QObject *parent = 0);

    bool getModifier(){return bModifier;}

    bool checkReleasedModifier(DWORD vkCode){
        if(vkCode == vkModifierCode)  {
            bModifier = false;
        }
        return bModifier;
    }

    void checkPressedModifier(DWORD vkCode){if(vkCode == vkModifierCode) bModifier = true;}


    int checkKey(DWORD vkCode, bool bGlobalModifier){
        if(vkCode == vkActivationKeyCode) {
           if(bModifier  == true || (vkModifierCode == 0x00 && bGlobalModifier == false)){
                int b_control = vkID / K_NUM;
                int k_action = vkID - b_control * K_NUM;
                if(b_control == B_ONOFF){
                    switch(k_action){
                        case K_DISABLE:
                            qDebug("vkCode K_DISABLE");
                            //emit doSetState(false);
                            break;
                        case K_ENABLE:
                            //qDebug("vkCode K_ENABLE");
                            //emit doSetState(true);
                            break;
                        case K_TOGGLE:
                            //qDebug("vkCode K_TOGGLE");
                            //emit doSetState(!ui->cbDongle->isChecked());
                            break;
                    }
                } else {
                    switch(k_action){
                        case K_DISABLE:
                            //qDebug("vkCode K_DISABLE");
                            //enableGroup(b_control-1,false);
                            break;
                        case K_ENABLE:
                            //qDebug("vkCode K_ENABLE");
                            //enableGroup(b_control-1,true);
                            break;
                        case K_TOGGLE:
                            //qDebug("vkCode K_TOGGLE");
                            //enableGroup(b_control-1,!keyenable2[b_control-1]->isChecked());
                            break;
                    }
                }
            }
           return vkID;
        }
        return 0xFF;
    }





private:
    int vkID;
    bool bModifier;
    DWORD vkModifierCode;
    DWORD vkActivationKeyCode;

signals:

public slots:
};

#endif // HOTKEY_H
