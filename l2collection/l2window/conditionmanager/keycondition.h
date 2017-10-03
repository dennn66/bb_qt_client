#ifndef KEYCONDITION_H
#define KEYCONDITION_H
#include <QString>
#include <QTimer>


#define idCP 0
#define idHP 1
#define idMP 2
#define idVP 3
#define idMobHP 4
#define idMobMP 5
#define idPet1HP 6
#define idPet2HP 7

//#define idTargetType 6 //Only for message to dongle
//#define idCheckSkillType 8 //Only for message to dongle

#define BARNUM 8

#define idCoolDown      0
#define idPause         1
#define idCondition     2
#define idSkillPause    3

#define CONDFNUM 4

#define idMinCP         0
#define idMinHP         1
#define idMinMP         2
#define idMinVP         3
#define idMinMobHP      4
#define idMinMobMP      5
#define idMinPet1HP     6
#define idMinPet2HP     7


#define idMaxCP         8
#define idMaxHP         9
#define idMaxMP         10
#define idMaxVP         11
#define idMaxMobHP      12
#define idMaxMobMP      13
#define idMaxPet1HP     14
#define idMaxPet2HP     15
#define idPauseSkillNum 16

#define CONDINUM 17

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

#define idCheckPet              11
#define idPetState              12

#define idCtrl                  13
#define idShift                 14

#define TokensNum               4
#define idCheckToken           15
#define idTokenCondition       idCheckToken+TokensNum

#define CONDBNUM idTokenCondition+TokensNum
//#define idNoUseSkillState      idTokenCondition+TokensNum
#define idNoUseSkillNum        48

//#define CONDBNUM idNoUseSkillState+idNoUseSkillNum

#define GROUPSNUM 4
#define TARGETTYPENUM 4

#define KEY_DB_SIZE         223

class KeyCondition: public QObject
{
    Q_OBJECT

public:
    KeyCondition(QString button, QObject *parent = 0);
    ~KeyCondition();

    unsigned char string2keycode(QString Key);

    void setGroupState(int group, bool state);
    bool getGroupState(int group);

    unsigned char getGroupsBinaryCondition();
    unsigned char getTargetTypeBinaryCondition();

    bool checkTokenCondition(int state){
        bool condition = true;
        for(int i = 0; i < TokensNum; i++){
            if(conditionb[idCheckToken+i]){
                if(conditionb[idTokenCondition+i] == (state == i)) return true;
                condition = false;
            }
        }
        return condition;
    }

    bool checkTargetCondition(unsigned char target){ return conditionb[idTargetMeOrPet+target];}

    bool checkBarCondition(int num, unsigned char bar){

        if(num <  BARNUM && bar <= 100 )
        {
            if(conditioni[num] <= 100 && bar <  conditioni[num]) return false;
            if(conditioni[BARNUM+num] <= 100 && bar >  conditioni[BARNUM+num]) return false;
        }
        return true;
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
