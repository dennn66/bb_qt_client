#ifndef DONGLEKEY_H
#define DONGLEKEY_H

#include <QObject>


namespace BadUSB {

class DongleKey : public QObject
{
    Q_OBJECT
public:
    explicit DongleKey(QObject *parent = 0);
    bool get_transfer_state(){return transfer_state;}
    void set_transfer_state(bool state){transfer_state = state;}
    bool get_condition_state(){return condition_state;}
    void set_condition_state(bool state){condition_state = state;}
    bool get_skill_state(){return skill_state;}
    void set_skill_state(bool state){skill_state = state;}

    void setup_key(int _index, unsigned char _key_code,
                   float _PauseTime, float _ReleaseTime,
                   float _ConditionTime,
                   bool _Ctrl, bool _Shift){
        index           = _index;
        key_code        = _key_code;
        PauseTime       = _PauseTime;
        ReleaseTime     = _ReleaseTime;
        ConditionTime   = _ConditionTime;
        Ctrl            = _Ctrl;
        Shift           = _Shift;
    }
    int             get_index()         {return index           ;}
    unsigned char   get_key_code()      {return key_code        ;}
    float           get_PauseTime()     {return PauseTime       ;}
    float           get_ReleaseTime()   {return ReleaseTime     ;}
    float           get_ConditionTime() {return ConditionTime   ;}
    bool            get_Ctrl()          {return Ctrl            ;}
    bool            get_Shift()         {return Shift           ;}



private:
    bool transfer_state;
    bool condition_state;
    bool skill_state;

    int index;
    unsigned char key_code;
    float PauseTime;
    float ReleaseTime;
    float ConditionTime;
    bool Ctrl;
    bool Shift;


signals:

public slots:
};
}
#endif // DONGLEKEY_H
