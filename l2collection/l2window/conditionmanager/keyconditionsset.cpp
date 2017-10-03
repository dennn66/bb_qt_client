#include "keyconditionsset.h"

const char* KeyConditionsSet::DefaultKeyDB[KEYNUM] = {"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8","F9","F10","F11",                   "F12",
                                    "1",  "2",  "3",  "4",  "5",  "6",  "7",  "8", "9", "0",  "-",                     "=",
                                    "P1", "P2", "P3", "P4", "P5", "P6", "P7", "P8","P9","P0", "KEYPAD_DOT_AND_DELETE", "KEYPAD_SLASH",
                                    "Q",  "W",  "E",  "R",  "T",  "Y",  "U",  "I", "O", "P",  "ESCAPE", "MOUSE_LEFT"};



KeyConditionsSet::KeyConditionsSet(QObject *parent) : QObject(parent)
{
    settings_file_name="default.cfg";
    nic = "<unnamed>";

    for(int i = 0;i < KEYNUM;i++){
        condition[i] = new KeyCondition(DefaultKeyDB[i]);
    }
}

KeyConditionsSet::~KeyConditionsSet()
{
    qDebug("KeyConditionsSet::~KeyConditionsSet()");
    for(int i = 0;i < KEYNUM;i++){
        delete condition[i];
    }
}


int KeyConditionsSet::LoadConfig(QString file_name){
    qDebug("KeyConditionsSet::LoadConfig");

    QSettings sett(file_name, QSettings::IniFormat);
    nic = sett.value("MAIN/NicName", "").toString();
    qDebug("Nic: %s", nic.toStdString().c_str());
    settings_file_name = file_name;
    qDebug("File: %s", file_name.toStdString().c_str());
    for(int i = 0;i < KEYNUM;i++){
        QString topic = "CONDITION";
        QTextStream topicstream(&topic);
        topicstream <<  i+1 << "/";

        QString var;
        QTextStream varstream(&var);
//        qDebug("topic: %s", topic.toStdString().c_str());
        for(int j = idCoolDown; j<CONDFNUM; j++){
            var = topic;
            varstream  << KeyCondition::conditionf_tag[j];
            condition[i]->setConditionF(j,sett.value(var.toStdString().c_str(), 0).toFloat());
        }

        for(int j = idMinCP; j<CONDINUM; j++){
            var = topic;
            varstream  << KeyCondition::conditioni_tag[j];
            QVariant v_cond = sett.value(var.toStdString().c_str(), 0xFF);

            //qDebug(v_cond.toString().toStdString().c_str());
            QString s_cond = v_cond.toString();
            if(strlen(s_cond.toStdString().c_str()) < 1){
                condition[i]->setConditionI(j, 0xFF);
            } else {
                condition[i]->setConditionI(j, s_cond.toInt());
            }
            //qDebug("Cond ID: %d = %d, \"%s\", %d", j, condition[i]->conditioni[j], s_cond.toStdString().c_str(), strlen(s_cond.toStdString().c_str()));
        }

        var = topic;
        varstream  << "$FSet";
        condition[i]->setState(sett.value(var.toStdString().c_str()).toBool());


        for(int j = 0; j<CONDBNUM; j++){
            var = topic;
            varstream  << KeyCondition::conditionb_tag[j];
            bool default_val = 0;
            if(j < idCheckStar) default_val = 1;
            condition[i]->setConditionB(j, sett.value(var.toStdString().c_str(), default_val).toBool());
        }


        var = topic;
        varstream  << "$Buttons";
        condition[i]->setKeyString(sett.value(var.toStdString().c_str(), ".").toString());
        condition[i]->setKeyCode(condition[i]->string2keycode(condition[i]->getKeyString()));
        qDebug("\"%s\",", condition[i]->getKeyString().toStdString().c_str());
    }

    return true;
}

int KeyConditionsSet::SaveConfig(QString file_name){
    qDebug("KeyConditionsSet::LoadConfig");

    QSettings sett(file_name, QSettings::IniFormat);

    sett.setValue("MAIN/NicName", nic);
    qDebug("Nic: %s", nic.toStdString().c_str());


    settings_file_name = file_name;
    qDebug("File: %s", file_name.toStdString().c_str());
    for(int i = 0;i < KEYNUM;i++){
        QString topic = "CONDITION";
        QTextStream topicstream(&topic);
        topicstream <<  i+1 << "/";

        QString var;
        QTextStream varstream(&var);
        for(int j = idCoolDown; j<CONDFNUM; j++){
            var = topic;
            varstream  << KeyCondition::conditionf_tag[j];
            QString value;
            QTextStream valuestream(&value);
            valuestream << condition[i]->getConditionF(j);
            sett.setValue(var.toStdString().c_str(), value.toStdString().c_str());
        }

        for(int j = idMinCP; j<CONDINUM; j++){
            var = topic;
            varstream  << KeyCondition::conditioni_tag[j];
            sett.setValue(var.toStdString().c_str(), condition[i]->getConditionI(j));
        }

        var = topic;
        varstream  << "$FSet";
        sett.setValue(var.toStdString().c_str(), condition[i]->getState());

        for(int j = 0; j<CONDBNUM; j++){
            var = topic;
            varstream  << KeyCondition::conditionb_tag[j];
            sett.setValue(var.toStdString().c_str(), condition[i]->getConditionB(j));
        }
        var = topic;
        varstream  << "$Buttons";
        sett.setValue(var.toStdString().c_str(), condition[i]->getKeyString());
    }
    return true;
}
