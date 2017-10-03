#include "conditionmanager.h"


ConditionManager::ConditionManager(QObject *parent) : QObject(parent)
{

    KeyConditionsSet* conditionSet = new KeyConditionsSet();
    cond_set_list.append(conditionSet);
    project_file_name = "default.bbproj";
    activeCondSet = 0;
}

/*

*/


bool ConditionManager::activateSettings(int index){
    if(!isValidIndex(index))return false;
    activeCondSet = index;
    return true;
}

KeyConditionsSet* ConditionManager::getSettings(int index){
    if(!isValidIndex(index))return NULL;
    return cond_set_list[index];
}

KeyConditionsSet* ConditionManager::getCurrentSettings(){
    if(!isValidIndex(activeCondSet))return NULL;
    return cond_set_list[activeCondSet];
}

QString ConditionManager::getCurrentSettings_file_name(){
    if(!isValidIndex(activeCondSet))return "none.cfg";
    return cond_set_list[activeCondSet]->settings_file_name;
}

bool ConditionManager::LoadProject(QString file_name){
    qDebug("L2Processor::LoadProject");
    QSettings sett(file_name, QSettings::IniFormat);

    project_file_name = file_name;
    qDebug("Proj File: %s", file_name.toStdString().c_str());
    int proj_size = sett.value("MAIN/count", 0).toInt();
    qDebug("count: %d", proj_size);
    if(proj_size < 1) return false;

    QString topic = "SETTINGS";

    for(int i = 0;i < proj_size;i++){
        QString var;
        QTextStream varstream(&var);
        var = topic;
        varstream  << "/settings_file_name" <<  i+1;
        qDebug("Topic: %s", var.toStdString().c_str());

        QString settings_file_name = sett.value(var.toStdString().c_str(), "default.cfg").toString();
        qDebug("Settings File: %s", settings_file_name.toStdString().c_str());

        if(i < cond_set_list.size()){
            activeCondSet = i;
            loadConfig(settings_file_name);
            qDebug("Load: %d", i);

        } else {
            AddConfig(settings_file_name);
            qDebug("Add: %d", i);
        }
        activeCondSet = 0;
    }
    while(proj_size < cond_set_list.size()){
        KeyConditionsSet* tmp = cond_set_list.last();
        qDebug("Removed node %s", tmp->settings_file_name.toStdString().c_str());
        cond_set_list.removeLast();
        delete(tmp);
        qDebug("Removed node %s", tmp->settings_file_name.toStdString().c_str());
    }
    return true;
}

bool ConditionManager::saveProject(QString file_name){

    qDebug("L2Processor::SaveProject");

    QSettings sett(file_name, QSettings::IniFormat);

    project_file_name = file_name;
    qDebug("File: %s", file_name.toStdString().c_str());
    sett.setValue("MAIN/count", cond_set_list.size());

    QString topic = "SETTINGS";

    for(int i = 0;i < cond_set_list.size();i++){
        QString var;
        QTextStream varstream(&var);
        var = topic;
        varstream  << "/settings_file_name" <<  i+1;
        sett.setValue(var.toStdString().c_str(), cond_set_list[i]->settings_file_name);
    }
    return true;
}

bool ConditionManager::AddConfig(QString file_name){
    qDebug("L2Processor::LoadConfig");
    KeyConditionsSet* conditionSet = new KeyConditionsSet();
    cond_set_list.append(conditionSet);
    activeCondSet = cond_set_list.size() - 1;
    getCurrentSettings()->LoadConfig(file_name);
    return true;
}

bool ConditionManager::loadConfig(QString file_name){
    qDebug("L2Processor::LoadConfig");
    getCurrentSettings()->LoadConfig(file_name);
    return true;
}

bool ConditionManager::saveConfig(QString file_name){

    qDebug("L2Processor::LoadConfig");
    getCurrentSettings()->SaveConfig(file_name);
    return true;
}

QString ConditionManager::getTitle(){
    QString title = "OK";
    QTextStream st(&title);
    //st << status;
    return title;
}

