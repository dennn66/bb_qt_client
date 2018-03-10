#ifndef KEYCONDITION_H
#define KEYCONDITION_H
#include <QString>
#include <QTimer>
#include <QDebug>


#define idCP 0
#define idHP 1
#define idMP 2
#define idVP 3
#define idMobHP 4
#define idMobMP 5
#define idPet1HP 6
#define idPet2HP 7
#define idPartyHP 8
#define idParty2HP 9
#define idParty3HP 10
#define idParty4HP 11
#define idParty5HP 12
#define idParty6HP 13


//#define idTargetType 6 //Only for message to dongle
//#define idCheckSkillType 8 //Only for message to dongle

#define BARNUM 14

#define idCoolDown      0
#define idPause         1
#define idCondition     2
#define idSkillPause    3

#define CONDFNUM 4

#define idMinCP         idCP        //0
#define idMinHP         idHP        //1
#define idMinMP         idMP        //2
#define idMinVP         idVP        //3
#define idMinMobHP      idMobHP     //4
#define idMinMobMP      idMobMP     //5
#define idMinPet1HP     idPet1HP    //6
#define idMinPet2HP     idPet2HP    //7
#define idMinMemberHP   idPartyHP   //8

#define CONDIGROUPS     idPartyHP+1 //9


#define idMaxCP         CONDIGROUPS + idCP      //9
#define idMaxHP         CONDIGROUPS + idHP      //10
#define idMaxMP         CONDIGROUPS + idMP      //11
#define idMaxVP         CONDIGROUPS + idVP      //12
#define idMaxMobHP      CONDIGROUPS + idMobHP   //13
#define idMaxMobMP      CONDIGROUPS + idMobMP   //14
#define idMaxPet1HP     CONDIGROUPS + idPet1HP  //15
#define idMaxPet2HP     CONDIGROUPS + idPet2HP  //16
#define idMaxMemberHP   CONDIGROUPS + idPartyHP //17

#define idPauseSkillNum idMaxMemberHP+1         //18

#define CONDINUM idPauseSkillNum + 1            //19

#define idGroupB1               0
#define idGroupB2               1
#define idGroupB3               2
#define idGroupB4               3

#define idCheckStar             4
#define idStarCondition         5

#define idTargetMeOrPet         6
#define idTargetChar            7
#define idTargetMob             8
#define idNoTarget              9

#define idCheckSkillTimeout     10

#define idCheckRange            11
#define idInRange               12

#define idCheckPet              13
#define idPetState              14

#define idCtrl                  15
#define idShift                 16

#define TokensNum               4
#define idCheckToken           17
#define idTokenCondition       idCheckToken+TokensNum

#define CONDBNUM idTokenCondition+TokensNum
//#define idNoUseSkillState      idTokenCondition+TokensNum
#define idNoUseSkillNum        48

//#define CONDBNUM idNoUseSkillState+idNoUseSkillNum

#define GROUPSNUM 4
#define TARGETTYPENUM 4

#define KEY_DB_SIZE         223

class KeyCondition
{
public:
    KeyCondition(QString button);
    ~KeyCondition();

    unsigned char string2keycode(QString Key);

    void setGroupState(int group, bool state);
    bool getGroupState(int group);

    unsigned char getGroupsBinaryCondition();
    unsigned char getTargetTypeBinaryCondition();

#define OR_MODE  true
#define AND_MODE false

    bool checkTokenCondition(int state){
        qDebug() << "bool checkTokenCondition(int state = " << state << ")";
        bool mode = OR_MODE;
        bool is_condition_on = false;

        for(int i = 0; i < TokensNum; i++){
            qDebug() << "check token " << i;
            if(conditionb[idCheckToken+i]){
                 qDebug() << "have condition on token " << i;
                is_condition_on = true;
                if(conditionb[idTokenCondition+i] == false) mode = AND_MODE;
                qDebug() << "if(conditionb[idTokenCondition+i] == false) mode =  " << mode;
            }
        }

        qDebug() << "is_condition_on =  " << is_condition_on;
        if(!is_condition_on) return true;

        if(mode == OR_MODE) {
             qDebug() << "OR_MODE";
            for(int i = 0; i < TokensNum; i++){
                if(conditionb[idCheckToken+i]){
                    qDebug() << "check token " << i;
                    if(conditionb[idTokenCondition+i] == (state == i)) {
                        qDebug() << "return true ";
                        return true;
                    }
                }
            }
            return false;
        } else { //AND_MODE
            qDebug() << "AND_MODE";
            for(int i = 0; i < TokensNum; i++){
                if(conditionb[idCheckToken+i]){
                    qDebug() << "check token " << i;
                    if (!conditionb[idTokenCondition+i] == (state == i)) {
                        qDebug() << "return false ";
                        return false;
                    }
                }
            }
            return true;
        }

    }

    bool checkTargetCondition(unsigned char target){ return conditionb[idTargetMeOrPet+target];}

    bool checkBarCondition(int bar_num, unsigned char bar_value, bool result_if_unknown = true){

        if(bar_num <  CONDIGROUPS && bar_value <= 100 )
        {
            if(conditioni[bar_num] <= 100 && bar_value <  conditioni[bar_num]) return false;
            if(conditioni[CONDIGROUPS+bar_num] <= 100 && bar_value >  conditioni[CONDIGROUPS+bar_num]) return false;
        } else {
            return result_if_unknown;
        }
        return true;
    }

    bool isBarConditionEnabled(int bar_num){
        if(bar_num <  CONDIGROUPS  && (conditioni[bar_num] < 100 || conditioni[CONDIGROUPS+bar_num] < 100)) return true;
        return false;
    }
    void setConditionB(int index, bool b){if(index >= 0 && index < CONDBNUM) conditionb[index] = b;}
    bool getConditionB(int index){ if(index >= 0 && index < CONDBNUM) return conditionb[index];  return false; }

    float getConditionF(int index){return conditionf[index];}
    void setConditionF(int index, float val){conditionf[index] = val;}

    float getCooldown4Dongle(){
        if(getConditionF(idCoolDown) > 0.9 && getConditionB(idCheckSkillTimeout)) return 0.25;
        return getConditionF(idCoolDown);
    }


    unsigned char   getConditionI(int index){return conditioni[index];}
    void setConditionI(int index, unsigned char   val){ conditioni[index] = val;}

    void setState(bool stt){FSet = stt;}
    bool getState(){return FSet;}

    void setKeyCode(unsigned char stt){KeyCode = stt;}
    unsigned char getKeyCode(){return KeyCode;}

    void setKeyString(QString stt){KeyString = stt;}
    QString getKeyString(){return KeyString;}

    const char* getKeyMnemonicDB(int index){return KeyMnemonicDB[index];}

    void selectKey(int index){
        KeyString = KeyMnemonicDB[index];
        KeyCode = KeyCodesDB[index];
    }

    static const char* conditionf_tag[CONDFNUM];
    static const char* conditioni_tag[CONDINUM];
    static const char* conditionb_tag[CONDBNUM];
    static const char* conditionb_name[CONDBNUM];

private:
    bool FSet;

    unsigned char KeyCode;
    QString KeyString;

    bool ctrl;
    bool shift;

    bool conditionb[CONDBNUM];
    float conditionf[CONDFNUM]; //Cooldown, Pause, ConditionIdle
    unsigned char  conditioni[CONDINUM]; //Min Max

    static const char* KeyMnemonicDB[KEY_DB_SIZE];
    static unsigned char KeyCodesDB[KEY_DB_SIZE];

private slots:

};

#endif // KEYCONDITION_H
