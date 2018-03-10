#include "hotkeys.h"

HotKeys::HotKeys(QObject *parent) : QObject(parent)
{

}

void HotKeys::process(){
    //LOAD CONFIG BB.ini
    QSettings sett("bb.ini", QSettings::IniFormat);
    bEnableHotKey = sett.value("MAIN/EnableHotKey", 0).toBool();
    WORD vkModifierCode = 0x00;
    WORD vkActivationKeyCode = 0x00;
    if(bEnableHotKey){
        // LOAD VK FROM CONFIG
        for(int i = 0; i< VK_NUM; i++){
            QString key_label = "MAIN/ModifierCode";
            QTextStream key_label_stream(&key_label);
            key_label_stream << i;
            QVariant v_cond = sett.value(key_label, 0x00);
            QString s_cond = v_cond.toString();

            if(s_cond.size() < 1){
                vkModifierCode = 0x00;
            } else {
                vkModifierCode = s_cond.toInt();
            }
            if(vkModifierCode > 0xFF)  vkModifierCode = 0x00;

            key_label = "MAIN/ActivationKeyCode";
            key_label_stream << i;

            v_cond = sett.value(key_label, 0x00);
            s_cond = v_cond.toString();
            if(s_cond.size() < 1){
                vkActivationKeyCode = 0x00;
            } else {
                vkActivationKeyCode = s_cond.toInt();
            }
            if(vkActivationKeyCode > 0xFF)  vkActivationKeyCode = 0x00;

            if(vkActivationKeyCode > 0){
                HotKey *hk = new HotKey(i, vkModifierCode, vkActivationKeyCode,  this);
                hklist.append(hk);
            }
        }

        // SET VK HOOK
        kb = SystemKeyboardReadWrite::instance();
        kb->setConnected(true);
        connect(kb, SIGNAL(keyPressed(DWORD )), SLOT(keyGlobalPressed(DWORD)));
        connect(kb, SIGNAL(keyReleased(DWORD)), SLOT(keyGlobalReleased(DWORD)));
    }
}



void HotKeys::keyGlobalPressed(DWORD vkCode)
{
    //qDebug("vkCode BoredomBreaker::keyGlobalPressed(DWORD vkCode=%d)", (int) vkCode);
    for(int i = 0; i<  hklist.size(); i++){
        hklist[i]->checkPressedModifier(vkCode);
    }
}

void HotKeys::keyGlobalReleased(DWORD vkCode)
{
    //qDebug("vkCode BoredomBreaker::keyGlobalReleased(DWORD vkCode=%d)", (int) vkCode);

    bool bIsItModifier = false;
    for(int i = 0; i< hklist.size(); i++){
        bIsItModifier = bIsItModifier || hklist[i]->checkReleasedModifier(vkCode);
    }
    //qDebug("vkCode bGlobalModifier = %d", bIsItModifier);
    if(bIsItModifier) return;




    for(int i = 0; i<  hklist.size(); i++){
        int vkID = hklist[i]->checkKey(vkCode);
        if(vkID > -1){
            int b_control = vkID / K_NUM;
            int k_action = vkID - b_control * K_NUM;

            if(b_control == B_SHIFT){
                switch(k_action){
                    case K_DISABLE:
                        //qDebug("vkCode K_DISABLE");
                        emit set_shift(false);
                        break;
                    case K_ENABLE:
                        //qDebug("vkCode K_ENABLE");
                        emit set_shift(true);
                        break;
                    case K_TOGGLE:
                        //qDebug("vkCode K_TOGGLE");
                        emit toggle_shift();
                        break;
                }
            } else if(b_control == B_ONOFF){
                switch(k_action){
                    case K_DISABLE:
                        //qDebug("vkCode K_DISABLE");
                        emit set_operation_state(false);
                        break;
                    case K_ENABLE:
                        //qDebug("vkCode K_ENABLE");
                        emit set_operation_state(true);
                        break;
                    case K_TOGGLE:
                        //qDebug("vkCode K_TOGGLE");
                        emit toggle_operation_state();
                        break;
                }
            } else {
                switch(k_action){
                    case K_DISABLE:
                         //qDebug("vkCode K_DISABLE");
                         emit setGroupState(b_control-1,false);
                         break;
                    case K_ENABLE:
                         //qDebug("vkCode K_ENABLE");
                         emit setGroupState(b_control-1,true);
                         break;
                    case K_TOGGLE:
                         //qDebug("vkCode K_TOGGLE");
                         emit toggleGroupState(b_control-1);
                         break;
                }
            }
        }
    }
}


