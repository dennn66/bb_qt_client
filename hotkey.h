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
#define B_SHIFT 5
#define B_NUM   6

#define VK_NUM K_NUM*B_NUM

class HotKey : public QObject
{
    Q_OBJECT
public:
    explicit HotKey(int id, DWORD modifierCode, DWORD activationKeyCode, QObject *parent = 0);

    bool checkReleasedModifier(DWORD vkCode){
        if(vkCode == vkModifierCode)  {
            bModifier = false;
            return true;
        }
        return false;
    }

    void checkPressedModifier(DWORD vkCode){if(vkCode == vkModifierCode) bModifier = true;}


    int checkKey(DWORD vkCode){
        if(vkCode == vkActivationKeyCode && bModifier) {
           return vkID;
        }
        return -1;
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
