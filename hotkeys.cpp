#include "hotkeys.h"

HotKeys::HotKeys(QObject *parent) : QObject(parent)
{
    //LOAD CONFIG BB.ini
    QSettings sett("bb.ini", QSettings::IniFormat);
    bEnableHotKey = sett.value("MAIN/EnableHotKey", 0).toBool();
    WORD vkModifierCode;
    WORD vkActivationKeyCode;
    bool bModifier;

    for(int i = 0; i< VK_NUM; i++){

        QString key_label = "MAIN/ModifierCode";
        QTextStream key_label_stream(&key_label);
        key_label_stream << i;
        QVariant v_cond = sett.value(key_label, 0x00);
        QString s_cond = v_cond.toString();
        if(strlen(s_cond.toStdString().c_str()) < 1){
            vkModifierCode = 0x00;
        } else {
            vkModifierCode = s_cond.toInt();
        }
        if(vkModifierCode > 0xFF)  vkModifierCode = 0x00;

        key_label = "MAIN/ActivationKeyCode";
        key_label_stream << i;

        v_cond = sett.value(key_label, 0x00);
        s_cond = v_cond.toString();
        if(strlen(s_cond.toStdString().c_str()) < 1){
            vkActivationKeyCode = 0x00;
        } else {
            vkActivationKeyCode = s_cond.toInt();
        }
        if(vkActivationKeyCode > 0xFF)  vkActivationKeyCode = 0x00;
        bModifier = false;
        if(vkActivationKeyCode > 0){
            HotKey *hk = new HotKey(i, bModifier, vkModifierCode, vkActivationKeyCode,  this);
            hklist.append(hk);
        }
    }




}
