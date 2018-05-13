#ifndef HOTKEYS_H
#define HOTKEYS_H

#include <QObject>
#include <QSettings>
#include <QTextStream>

#include "hotkey.h"





class HotKeys : public QObject
{
    Q_OBJECT
public:
    explicit HotKeys(QObject *parent = 0);

    void keyPressed(DWORD vkCode){

        for(int i = 0; i<  hklist.size(); i++){
            hklist[i]->checkPressedModifier(vkCode);
        }
    }

    int keyReleased(DWORD vkCode){
        bool bGlobalModifier = false;
        for(int i = 0; i< hklist.size(); i++){
            bGlobalModifier = bGlobalModifier || hklist[i]->checkReleasedModifier(vkCode);
        }
        qDebug("vkCode bGlobalModifier = %d", bGlobalModifier);


        for(int i = 0; i<  hklist.size(); i++){

            int vkID = hklist[i]->checkKey(vkCode, bGlobalModifier);
            if(vkID < 0xFF) return vkID;
        }
        return 0xFF;

    }

private:
    QVector <HotKey*> hklist;
    bool bEnableHotKey;
signals:

public slots:
};

#endif // HOTKEYS_H
