#include "dongle.h"



Dongle::Dongle(QObject *parent) : QObject(parent)
{
//    currentl2w = NULL;
    state = STATE_DISCONNECTED;
    handle = NULL;
    activity = DO_NORMAL;
    target_device_state         = 0b00000000;
    current_device_state = 0b00000000;


    //LOAD CONFIG BB.ini
    //QSettings sett("bb.ini", QSettings::IniFormat);


    //bEnableTimingDebug = sett.value("MAIN/EnableTimingDebug", 0).toBool();

    for(int i = 0; i < KEYNUM; i++) key[i].set_transfer_state(false);
}

void Dongle::spin()
{
    qDebug("Spin dongle");

    if(state == STATE_DISCONNECTED){
        connect();
    } else {
        if(state == STATE_ON || state == STATE_OFF) {
            switch (activity) {
            case DO_WRITEKEYTODONGLE:
                for(int i = 0; i< KEYNUM; i++){
                    if(!key[i].get_transfer_state()){
                        sendKeyToDongle(i);
                        #ifdef WIN32
                            Sleep(50);
                        #else
                            usleep(50*1000);
                        #endif
                    }
                }
                activity = DO_NORMAL;
                break;
            case DO_JUMPTOBOOTLOADER:
                sendCMD_JUMP_TO_BOOTLOADER();
                state = STATE_DISCONNECTED;
                disconnect();
                break;
            case DO_SETDEVICESTATE:
                sendCMD_SET_STATE();
                activity = DO_NORMAL;
                break;
            case DO_NORMAL:
                sendCMD_SET_SKILL_STATE();
                recieve_status() ;
                break;
            default:
                break;
            }
         }
    }
}

void Dongle::process()
{
    qDebug("Start dongle checker");
    sendCMD_DELETE_ALL();
    while(1){
        QTime updateTime;
        updateTime.start();
        int delta;

        try {
            spin();
        } catch(...) {
            qDebug("Spin dongle failed");
            state = STATE_DISCONNECTED;
        }

        emit showStatus(current_device_state, updateTime.elapsed());
        delta = 20 - updateTime.elapsed();
        delta = (delta > 0)?delta:0;
        #ifdef WIN32
            Sleep(delta);
        #else
            usleep(delta*1000);
        #endif

    }
    emit finished();
}





// SLOTS ##################################################

void Dongle::jump_to_bootloader(){
    activity = DO_JUMPTOBOOTLOADER;
}

void Dongle::setup_key(int index, bool state, unsigned char Key, float PauseTime, float ReleaseTime, float ConditionTime,  bool Ctrl, bool Shift){
    qDebug("void Dongle::setup_key(int index = %d, unsigned char Key, float PauseTime, float ReleaseTime, float ConditionTime, unsigned int groups, bool Ctrl, bool Shift)",  index);

    key[index].setup_key( index,  Key,  PauseTime,  ReleaseTime,  ConditionTime, Ctrl,  Shift);
    key[index].set_condition_state(state);
    key[index].set_transfer_state(false);
    activity = DO_WRITEKEYTODONGLE;
    qDebug("FIN void Dongle::setup_key");
}


void Dongle::set_modifier(bool bCtrl, bool bShift){
    qDebug("Dongle::doSetModifier(bool bCtrl, bool bShift): %d %d", bCtrl, bShift);
    unsigned char state = (bCtrl)?(1 << DEVICE_CTRL):0;
    state = (bShift)?(state | (1 << DEVICE_SHIFT)):state;
    setDeviceState((getDeviceState() & (~((1 << DEVICE_CTRL) | (1 << DEVICE_SHIFT)))) | state);
    activity = DO_SETDEVICESTATE;
}

void Dongle::set_mode(bool bMode){
    qDebug("DongleWorker::doSetMode(bool bMode): %d", bMode);
    unsigned char state = (bMode)?(1 << DEVICE_MODE):0;
    setDeviceState((getDeviceState() & (~(1 << DEVICE_MODE))) | state);
    activity = DO_SETDEVICESTATE;
}

void Dongle::set_operation_state(bool stt){
    unsigned char state = (stt)?(1 << DEVICE_STATUS):0;
    setDeviceState((getDeviceState() & (~(1 << DEVICE_STATUS))) | state);
    activity = DO_SETDEVICESTATE;
}

// COMBO CMD ##################################################

void Dongle::sendKeyToDongle(int condition_index){
    qDebug("Dongle::sendKeyToDongle(int condition_index, bool state)");

     if(key[condition_index].get_condition_state()){
        sendCMD_ADD_NODE(
                    key[condition_index].get_index(), //key index
                    key[condition_index].get_key_code(),
                    key[condition_index].get_PauseTime(),    //    cond->getConditionF(idPause),
                    key[condition_index].get_ReleaseTime(),    //    cond->getCooldown4Dongle(),
                    key[condition_index].get_ConditionTime(),    //    cond->getConditionF(idCondition),
                    key[condition_index].get_Ctrl(),      // cond->getConditionB(idCtrl), // ctrl,
                    key[condition_index].get_Shift()      // cond->getConditionB(idShift) //shift
             );
    } else {
        sendCMD_DELETE_NODE(condition_index);
    }
    key[condition_index].set_transfer_state(true);
}

// LOW LVL CMD ##################################################


void Dongle::sendCMD_ADD_NODE(int index, unsigned char key_code, float PauseTime, float ReleaseTime, float ConditionTime,  bool Ctrl, bool Shift){
    unsigned char hpbuf[OUT_REPORT_SIZE-3];
    memset(hpbuf,0,sizeof(hpbuf));

    hpbuf[0] = index;                                                           //0
    hpbuf[1] = key_code;                                                        //1
    *((uint32_t*)(hpbuf+2)) = (uint32_t)(PauseTime*1000);  //; 1 ms grade           //2, 3, 4, 5
    *((uint32_t*)(hpbuf+6)) = (uint32_t)(ReleaseTime*1000);  //; 1 ms grade         //6, 7, 8, 9
    *((uint32_t*)(hpbuf+10)) = (uint32_t)(ConditionTime*1000);  //; 1 ms grade      //10, 11, 12, 13
    hpbuf[14] = 0b11111111;                                                         //14 groups
    hpbuf[15] = 0;                                                              //15

    if(Ctrl)      hpbuf[15] |= 0b00001000; // Ctrl
    if(Shift)     hpbuf[15] |= 0b00000100; // Shift


    //qDebug("CMD_ADD_NODE %s, Cooldown (0.1s) %d, Abs (0.5s) %d, Cond (0.5s) %d", Key.toStdString().c_str(), hpbuf[2]+hpbuf[3]*256, hpbuf[1], hpbuf[4]);
    send_command(SERVICE_CONFIG, CMD_ADD_NODE, hpbuf);
}

void Dongle::sendCMD_DELETE_NODE(int index){
    unsigned char hpbuf[OUT_REPORT_SIZE-3];
    memset(hpbuf,0,sizeof(hpbuf));
    hpbuf[0] = index & 0xFF;
    send_command(SERVICE_CONFIG, CMD_DELETE_NODE, hpbuf);
}


void Dongle::sendCMD_SET_STATE() {
    qDebug("Dongle::sendCMD_SET_STATE");
    unsigned char hpbuf[OUT_REPORT_SIZE-3];
    memset(hpbuf,0,sizeof(hpbuf));
    hpbuf[0] = 0b11111111;
    send_command(SERVICE_DEVICE, target_device_state, hpbuf);
}


void Dongle::sendCMD_SET_SKILL_STATE(){
    unsigned char hpbuf[OUT_REPORT_SIZE-3];
    qDebug("Dongle::sendCMD_SET_SKILL_STATE");
    //memset(hpbuf,0,sizeof(hpbuf));
    hpbuf[0] = 0;
    hpbuf[1] = 0;
    hpbuf[2] = 0;
    hpbuf[3] = 0;
    hpbuf[4] = 0;
    hpbuf[5] = 0;
    hpbuf[6] = 0;

    for(int i = 0; i < KEYNUM; i++){  //48 keys
        if(key[i].get_condition_state()){
            if(key[i].get_skill_state()) {
                int buf_byte = i >> 3;
                int buf_bit = i - (buf_byte << 3);
                unsigned char t = hpbuf[buf_byte] | ((unsigned char)(1 << buf_bit));
                hpbuf[buf_byte] =   t;
                qDebug("Skill  %d = %d * 8 + %d = %d", i, buf_byte, buf_bit, hpbuf[buf_byte]);
            }
        }
    }
    qDebug("Skill State %d %d %d %d %d %d", hpbuf[0], hpbuf[1], hpbuf[2], hpbuf[3], hpbuf[4], hpbuf[5]);
    send_command(SERVICE_CONFIG, CMD_SET_SKILL_STATE, hpbuf);
}

void Dongle::sendCMD_DELETE_ALL(){
    unsigned char hpbuf[OUT_REPORT_SIZE-3];
    memset(hpbuf,0,sizeof(hpbuf));
    send_command(SERVICE_CONFIG, CMD_DELETE_ALL, hpbuf);
}

void Dongle::sendCMD_JUMP_TO_BOOTLOADER(){
    unsigned char hpbuf[OUT_REPORT_SIZE-3];
    memset(hpbuf,0,sizeof(hpbuf));
    send_command(SERVICE_CONFIG, CMD_JUMP_TO_BOOTLOADER, hpbuf);
}

// DIRECT KEY TRANSFER ##################################################

int Dongle::set_key_report(unsigned char key_code, bool Ctrl, bool Shift)
{
    qDebug("int Dongle::send_key(QString Key, bool Ctrl, bool Shift)command");

    unsigned char hpbuf[OUT_REPORT_SIZE-3];
    memset(hpbuf,0,sizeof(hpbuf));
     // Modifiers:
    if(Ctrl) hpbuf[5]  |= HID_KEYBOARD_MODIFIER_LEFTCTRL;
    if(Shift) hpbuf[5]  |= HID_KEYBOARD_MODIFIER_LEFTSHIFT;
    send_command(SERVICE_CONFIG, key_code, hpbuf);
    return 1;
}

int Dongle::send_key(unsigned char key_code, bool Ctrl, bool Shift)
{
    qDebug("int Dongle::send_key(QString Key, bool Ctrl, bool Shift)command");
    set_key_report(key_code, Ctrl, Shift);
    #ifdef WIN32
        Sleep(60);
    #else
        usleep(60*1000);
    #endif
    set_key_report(0x00, false, false);

    return(state);
}


// USB COMMANDS

void Dongle::recieve_status() {
    qDebug("Dongle::recieve_status");
    unsigned char buf[IN_REPORT_SIZE];

    if(state == STATE_DISCONNECTED) return;
    memset(buf,0,sizeof(buf));

    buf[0] = 0;
    int res;
    try {
        res =  hid_get_input_report(handle, buf, sizeof(buf));
    } catch(...) {
        res = -1;
        state = STATE_DISCONNECTED;
    }

    if (res == -1)
    {
        qDebug("Ошибка при  приёме данных");
        disconnect();
    } else {
        current_device_state = buf[1] & DEVICE_MASK;
        if((current_device_state & (1 << DEVICE_STATUS)) == 0) {
            state = STATE_OFF;
        } else {
            state = STATE_ON;
        }
        //current_group_state = buf[2];

    }
}

int Dongle::connect()
{
    qDebug("Dongle::connect()");

    // Open the device using the VID, PID,
    // and optionally the Serial number.
    //handle = hid_open(0x03eb, 0x204d, L"12345");
    try {
        handle = hid_open(0x03eb, 0x204d, NULL);
    } catch(...) {
        qDebug("unable to open device exeption\n");
        state = STATE_DISCONNECTED;
    }
    if (!handle) {
        qDebug("unable to open device\n");
        state = STATE_DISCONNECTED;
        return state;
    }
    // Set the hid_read() function to be non-blocking.

    try {
        hid_set_nonblocking(handle, 1);
    } catch(...) {
       qDebug("unable to set hid_set_nonblocking\n");;
       state = STATE_DISCONNECTED;
    }
    state = STATE_OFF;
    activity = DO_WRITEALLTODONGLE;
    return state;
}

int Dongle::disconnect()
{
    qDebug("Dongle::disconnect()");

    try {
        hid_close(handle);
    } catch(...) {
        ;;
        qDebug("hid_close failed\n");
        state = STATE_DISCONNECTED;

    }

    /* Free static HIDAPI objects. */

    try {
        hid_exit();
    } catch(...) {
        ;;
        qDebug("hid_exit failed\n");
        state = STATE_DISCONNECTED;

    }

    state = STATE_DISCONNECTED;
    return STATE_DISCONNECTED;
}

int Dongle::send_command(int device, int command, unsigned char* cmd_arg)
{
    qDebug("Dongle::send_command(int device, int command, unsigned char* cmd_arg)command: %d", command);
    int res;
    unsigned char buf[OUT_REPORT_SIZE];

    if(state != STATE_DISCONNECTED && state != STATE_TRANSMIT_CONF){
        // Set up the command buffer.
        memset(buf,0,sizeof(buf));
        buf[0] = 0x00;
        buf[1] = device;  //INTERFACE_ID_Keyboard
        buf[2] = command;// CMD_SET_HPCPMP
        for(int i = 0; i< OUT_REPORT_SIZE-3; i++) buf[i+3] = cmd_arg[i];
        int current_state = state;
        state = STATE_TRANSMIT_CONF;

        try {
            res = hid_write(handle, buf, sizeof(buf));
        } catch(...) {
            res = -1;
        }
        if (res < 0) {
            qDebug("Unable to write()\n");
            return disconnect();
        }
        state = current_state;
    }
    return(state);
}


