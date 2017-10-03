#ifndef L2COLLECTION_H
#define L2COLLECTION_H

#include <QObject>
#include <QTime>

#include "l2collection\l2window\l2window.h"
#include "l2collection\l2window\conditionmanager\keysettingsdialog.h"
#include "dongle\dongle.h"
#include "dongle\dongleworker.h"

Q_DECLARE_METATYPE(QVector <HWND>)

class L2Collection : public QObject
{
    Q_OBJECT
public:
    explicit L2Collection(QObject *parent = 0);
    bool isValidIndex(int index);
    bool isValidConditionIndex(int index){
        if(!isValidIndex(activeIndex)) return false;
        return l2list[activeIndex]->conditionmanager->isValidIndex(index);
    }
    int getCond_list_size(){
        if(!isValidIndex(activeIndex)) return 0;
        if(l2list[activeIndex]->conditionmanager->cond_set_list.isEmpty()) return 0;
        return l2list[activeIndex]->conditionmanager->cond_set_list.size();
    }

    QString getNic(){ return getNic(getActiveCondSet());}
    QString getNic(int index){if(!isValidIndex(activeIndex)) return "0"; return l2list[activeIndex]->conditionmanager->getNic(index);}
    HWND getHWND(int l2windownum){if(!isValidIndex(l2windownum)) return 0; return l2list[l2windownum]->getHWND();}
    void enableCondition(int index, bool checked){
        if(!isValidIndex(activeIndex)) return;
        l2list[activeIndex]->conditionmanager->getCurrentSettings()->condition[index]->setState(checked);
        emit doSendKeyToDongle(index);
    }

   int getActiveCondSet(){if(!isValidIndex(activeIndex)) return 0; return l2list[activeIndex]->conditionmanager->activeCondSet;}

    QString getProject_file_name(){if(!isValidIndex(activeIndex)) return "noProject"; return l2list[activeIndex]->conditionmanager->project_file_name;}
    QString getCurrentSettings_file_name(){if(!isValidIndex(activeIndex)) return "noSettings"; return l2list[activeIndex]->conditionmanager->getCurrentSettings_file_name();}
    void activateSettings(int index){if(!isValidIndex(activeIndex)) return;l2list[activeIndex]->conditionmanager->activateSettings(index);}

    QString getConditionLabel(int i){if(!isValidIndex(activeIndex)) return "_"; return l2list[activeIndex]->conditionmanager->getConditionLabel(i);}
//    bool getConditionState(int i){if(!isValidIndex(activeIndex)) return false;return l2list[activeIndex]->conditionmanager->getConditionState(i);}

    QIcon* getIcon(){if(!isValidIndex(activeIndex)) return NULL; return l2list[activeIndex]->l2parser->getIcon();}
    QString getTitle(int l2index){if(!isValidIndex(l2index)) return "noTitle"; return l2list[l2index]->l2parser->getTitle();}
    QString getCurrentTitle(){return getTitle(activeIndex);}
    int getTargetType(){if(!isValidIndex(activeIndex)) return NOTARGET; return l2list[activeIndex]->l2parser->getTargetType();}
    int getXP(int barnum){if(!isValidIndex(activeIndex)) return XP_ERR; return l2list[activeIndex]->l2parser->getXP(barnum);}
    QColor getTokenColor(){if(!isValidIndex(activeIndex)) return Qt::white;return *l2list[activeIndex]->l2parser->getTokenColor();}
    bool getTokenState(){if(!isValidIndex(activeIndex)) return false; return l2list[activeIndex]->l2parser->getTokenState();}

    bool isActionConditionRdy(int keynum){if(!isValidIndex(activeIndex)) return false; return l2list[activeIndex]->isActionConditionRdy(keynum);}
    bool isActionTurnedOn(int keynum){if(!isValidIndex(activeIndex)) return false; return l2list[activeIndex]->isActionTurnedOn(keynum);}
    bool isActionlRdyToApply(int keynum){if(!isValidIndex(activeIndex)) return false; return l2list[activeIndex]->isActionlRdyToApply(keynum);}

    int get_l2list_size(){return l2list.size();}

    void addL2Window(HWND hwnd);
    int getActiveL2W(){return activeIndex;}
    void getStatusBtn(QImage* imgStatus, bool pressed){if(!isValidIndex(activeIndex)) return; l2list[activeIndex]->l2parser->getStatusBtn(imgStatus,  pressed);}
    L2Window* currentl2w(){if(!isValidIndex(activeIndex)) return NULL; return l2list[activeIndex];}
    void spinonce(); //+

//        L2engine* l2_engine;
private:
    QString default_file_name;
    int activeIndex;
    DongleWorker* dongle_worker;


protected:
    QVector <L2Window*> l2list;


signals:
    void finished(); //+
    void error(QString err); //+

    void isL2Active(bool isActive, int right, int top);
    void showParserStatus(int updatetime);

    void resetWinView(); //+
    void resetSetView(); //+
    void resetKeyView(); //+

    void doSendKeyToDongle(int);
    void doSaveAllToDongle();
    void showDongleStatus(unsigned char d_stt, unsigned char g_stt, int updatetime); /* */



public slots:
    void resetL2Windows(QVector <HWND> *hwnd_list); //+

    void setDongleState(bool){;} //fix
    void setModifiers(bool, bool){;} //fix
    void saveAllToDongle(){emit doSaveAllToDongle();}//fix
    void rebootDongle(){;}//fix

    void setActiveL2W(int index); //+
    void findBars();

    void setActiveSettings(int index); //+
    void setNic(QString); //l2collection->setNic(text);
    void addConfig(QString);
    void loadConfig(QString);
    void saveConfig(QString);
    void loadProject(QString);
    void saveProject(QString);
    void setConditionState(int, bool);
    void editCondition(int);

    void setGroupState(int, bool){;}//fix
    void turn_completed(bool result, int ellipsedtime){
        qDebug("turn_completed(bool result, int ellipsedtime) %d, %d",  result,  ellipsedtime);
        qDebug("Thread: %d", (int) QThread::currentThreadId());

        if(result) {
            emit isL2Active( currentl2w()->l2parser->isActiveL2Window, currentl2w()->l2parser->windowtopright.rx(), currentl2w()->l2parser->windowtopright.ry());
            emit showParserStatus(ellipsedtime);
        } else {
            emit isL2Active( false, 0, 0);
        }
    }


};

#endif // L2COLLECTION_H
