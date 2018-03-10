#ifndef DONGLE_H
#define DONGLE_H

#include <windows.h>
#include <hidapi.h>
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <QThread>
#include <QTime>
#include <QMessageBox>
#include <QTextStream>
#include <QSettings>

#define KEYNUM 48
//#include "l2window.h"
#include "donglekey.h"

#define CMD_UNUSED1             0
#define CMD_UNUSED2             1
#define CMD_ADD_NODE            2
#define CMD_DELETE_NODE         3
#define CMD_DELETE_ALL          4
#define CMD_JUMP_TO_BOOTLOADER  5
#define CMD_UNUSED3             6
#define CMD_UNUSED4             7
#define CMD_UNUSED5             8
#define CMD_SET_SKILL_STATE     9


#define SERVICE_CONFIG   0     /**< set Keyboard msg */
#define SERVICE_MOUSE      1     /**< set Mouse msg */
#define SERVICE_DEVICE     2     /**< set Device operating mode */
#define SERVICE_KEYBOARD     3     /**< set Device config */

#define STATE_DISCONNECTED     -1
#define STATE_OFF               0
#define STATE_ON                1
#define STATE_TRANSMIT_CONF     2
#define STATE_TRANSMIT_KEY      3


#define DO_NORMAL               0
#define DO_WRITEALLTODONGLE     1
#define DO_WRITEKEYTODONGLE     2
#define DO_SETDEVICESTATE       3
#define DO_JUMPTOBOOTLOADER     4

#define OUT_REPORT_SIZE       19
#define IN_REPORT_SIZE        3

#define 	HID_KEYBOARD_MODIFIER_LEFTCTRL   (1 << 0)
#define 	HID_KEYBOARD_MODIFIER_LEFTSHIFT   (1 << 1)



// Set device status
// 0 - Device Status
// 1 - Device Mode
// 2 - Group 0 status
// 3 - Group 1 status
// 4 - Group 2 status
// 5 - Group 3 status
// 6 - Ctrl state
// 7 - Shift state
#define DEVICE_STATUS	0
#define DEVICE_MODE		1

#define DEVICE_CTRL		6
#define DEVICE_SHIFT	7
#define DEVICE_MASK      0b11111111


#define USB_MOUSE_BTN_MASK      0x1F
#define USB_MOUSE_BTN_LEFT      0
#define USB_MOUSE_BTN_RIGHT     1
#define USB_MOUSE_BTN_MIDDLE    2
#define USB_MOUSE_BTN_4th       3
#define USB_MOUSE_BTN_5th       4

class Dongle : public QObject
{
    Q_OBJECT

public:
    explicit Dongle(QObject *parent = 0);

    void sendKeyToDongle(int condition_index);
    void setDeviceState(unsigned char state){
        //qDebug("Dongle::setDeviceState(unsigned int state): %d", state);
        target_device_state = state;
        activity = DO_SETDEVICESTATE;
    }
    unsigned char getDeviceState(){return current_device_state;}


 public slots:
    void process();

private:
    hid_device *handle;
    int state;
 //   bool bEnableTimingDebug;
 //   unsigned char target_group_state;
 //   unsigned char current_group_state;

    unsigned char target_device_state;
    unsigned char current_device_state;
    int activity;
    DongleKey key[KEYNUM];
    void spin();

    int connect();
    int disconnect();
    void recieve_status();
    int send_key(unsigned char , bool Ctrl, bool Shift);
    int set_key_report(unsigned char key_code, bool Ctrl, bool Shift);
    int send_command(int device, int command, unsigned char* cmd_arg);

    void sendCMD_SET_STATE();
    void sendCMD_ADD_NODE(int index, unsigned char Key, float PauseTime, float ReleaseTime, float ConditionTime,  bool Ctrl, bool Shift);
    void sendCMD_DELETE_NODE(int index);
    void sendCMD_DELETE_ALL();
    void sendCMD_SET_SKILL_STATE();
    void sendCMD_JUMP_TO_BOOTLOADER();
public slots:
    void setup_key(int index,  bool state, unsigned char Key, float PauseTime, float ReleaseTime, float ConditionTime,  bool Ctrl, bool Shift);
    void set_dongle_skill_state(int num, bool state){key[num].set_skill_state(state);}
    void set_operation_state(bool stt);
    void toggle_operation_state();
    void set_modifier(bool bCtrl, bool bShift);
    void set_shift(bool bShift);
    void toggle_shift();
    void set_ctrl(bool bCtrl);
    void toggle_ctrl();
    void set_mode(bool bMode);
    void jump_to_bootloader();
    int set_mouse_report(qint8 x, qint8 y, bool left, bool right, bool middle);


signals:
    void finished();
    void error(QString err);
    void showStatus(unsigned char d_stt,  int updatetime);


};

#endif // DONGLE_H
