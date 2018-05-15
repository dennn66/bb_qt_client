#include "conditionmgr.h"

ConditionMgr::ConditionMgr( QObject *parent) : QObject(parent)
{
    //LOAD CONFIG BB.ini
    QSettings sett("bb.ini", QSettings::IniFormat);

    KeyConditionsSet* conditionSet = new KeyConditionsSet();
    cond_set_list.append(conditionSet);
    project_file_name = "default.bbproj";
    activeCondSet = 0;

}


bool ConditionMgr::activateSettings(int index){
    if(!isValidIndex(index))return false;
    activeCondSet = index;
    return true;
}

KeyConditionsSet* ConditionMgr::getCurrentSettings(){
    if(!isValidIndex(activeCondSet))return NULL;
    return cond_set_list[activeCondSet];
}


int ConditionMgr::LoadProject(QString file_name){
    //qDebug("L2Window::LoadProject");
    QSettings sett(file_name, QSettings::IniFormat);

    project_file_name = file_name;
    //qDebug("Proj File: %s", file_name.toStdString().c_str());
    int proj_size = sett.value("MAIN/count", 0).toInt();
    //qDebug("count: %d", proj_size);
    if(proj_size < 1) return 1;

    QString topic = "SETTINGS";

    for(int i = 0;i < proj_size;i++){
        QString var;
        QTextStream varstream(&var);
        var = topic;
        varstream  << "/settings_file_name" <<  i+1;
        //qDebug("Topic: %s", var.toStdString().c_str());

        QString settings_file_name = sett.value(var.toStdString().c_str(), "default.cfg").toString();
        //qDebug("Settings File: %s", settings_file_name.toStdString().c_str());

        if(i < cond_set_list.size()){
            activeCondSet = i;
            LoadConfig(settings_file_name);
            //qDebug("Load: %d", i);

        } else {
            AddConfig(settings_file_name);
            //qDebug("Add: %d", i);
        }
        activateSettings(0);
    }
    while(proj_size < cond_set_list.size()){
        KeyConditionsSet* tmp = cond_set_list.last();
        //qDebug("Removed node %s", tmp->settings_file_name.toStdString().c_str());
        cond_set_list.removeLast();
        delete(tmp);
        //qDebug("Removed node %s", tmp->settings_file_name.toStdString().c_str());
    }
    //qDebug("FIN L2Window::LoadProject");

    return 1;
}

int ConditionMgr::SaveProject(QString file_name){

    //qDebug("L2Window::SaveProject");

    QSettings sett(file_name, QSettings::IniFormat);

    project_file_name = file_name;
    //qDebug("File: %s", file_name.toStdString().c_str());
    sett.setValue("MAIN/count", cond_set_list.size());

    QString topic = "SETTINGS";

    for(int i = 0;i < cond_set_list.size();i++){
        QString var;
        QTextStream varstream(&var);
        var = topic;
        varstream  << "/settings_file_name" <<  i+1;
        sett.setValue(var.toStdString().c_str(), cond_set_list[i]->settings_file_name);
    }
    return 1;
}

int ConditionMgr::AddConfig(QString file_name){
    //qDebug("L2Window::LoadConfig");
    KeyConditionsSet* conditionSet = new KeyConditionsSet();
    cond_set_list.append(conditionSet);
    activeCondSet = cond_set_list.size() - 1;
    getCurrentSettings()->LoadConfig(file_name);
    activateSettings(activeCondSet);
    return 1;
}

int ConditionMgr::LoadConfig(QString file_name){
    //qDebug("L2Window::LoadConfig");
    getCurrentSettings()->LoadConfig(file_name);
    //qDebug("FIN L2Window::LoadConfig");

    return 1;
}

int ConditionMgr::SaveConfig(QString file_name){

    //qDebug("L2Window::LoadConfig");
    getCurrentSettings()->SaveConfig(file_name);
    return 1;
}

