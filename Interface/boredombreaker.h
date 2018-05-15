#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWinExtras/QtWin>
#include <QMessageBox>
#include <QTextStream>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QThread>
#include <QtMultimedia/QSound>
#include <QtMultimedia/QMediaPlayer>
#include <QProgressBar>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QListWidget>

#include "l2window.h"
#include "dongle.h"
#include "l2parser.h"
#include "keysettingsdialog.h"
#include "clicker.h"
#include "hotkeys.h"
#include "telegrambot.h"
#include "eventprocessor.h"




BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lParam);

namespace Ui {
class BoredomBreaker;
}

class BoredomBreaker : public QMainWindow
{
    Q_OBJECT

public:
    explicit BoredomBreaker(QWidget *parent = 0);
    ~BoredomBreaker();
    void addL2HWND(HWND hwnd);

private:
    Ui::BoredomBreaker *ui;
//    L2Window* currentl2w;
//    int currentl2i;

//    QLabel *keylabel[KEYNUM];
//    QCheckBox *keyenable[KEYNUM];
    QCheckBox *keyenable2[GROUPSNUM];
//    QPushButton *keysettings[KEYNUM];

    BadUSB::Dongle* dongle;
    Clicker* clicker;
    L2parser* l2_parser;
    TelegramBot* tele_bot;
    EventProcessor* evprocessor;

    QVector <HWND> hwnd_list;

    QString project_file_name;
    QString settings_file_name;
    QListWidget* lstAllSkills;
    QListWidgetItem *listNoUseSkill[KEYNUM];

    HotKeys* hk;

    bool bEnableSound;
    bool bEnableTelegram;

    int ellipsed_time;
    static const char* StyleSheetCheckBox[5];
    static const char* StyleSheetLabel[6];

//    bool isValidL2W();
    void updateListbox(QVector <QString>  list, QComboBox* combo, int current_index);

//    L2Window*   getCurrentL2W();
//    int         getCurrentL2index();


public slots:
    void itemDoubleClicked(QListWidgetItem* item);
    void itemClicked(QListWidgetItem* item);
    void showDongleStatus(unsigned char d_stt, int updatetime); /* */
    void showParserStatus(int updatetime, L2Window* l2w, QImage clicker_bk);
    void cbKeyEnableBxClicked(bool checked);
    void pbLoadClicked();
    void pbSaveProjectClicked();
    void pbLoadProjectClicked();
    void pbSaveClicked();
    void pbAddClicked();
    void cmbWinListActivated(int index);
    void cmbCondSetListActivated(int index);
    void popupWindow(HWND hwnd);
    void popupBbWindow();
    void updateGroupState(int num,  bool state);

    void updateConditiosList(QVector <QString> list, int current_index, QString file_name);
    void updateL2WindowsList(QVector <QString> list, int current_index, QString file_name);


//    void setActiveL2W(L2Window* l2w, int l2_index);
    void startL2enumerating();


signals:
    void toggleRuleState(int key_index);
    void editRule(int key_index);
    void changeNic(const QString &text);
    void send_all_keys_to_dongle();


    void setGroupState(int i, bool state);

    void set_operation_state(bool stt);
    void set_modifier(bool bCtrl, bool bShift);
    void setup_key(int index,  bool state, unsigned char Key, float PauseTime, float ReleaseTime, float ConditionTime,  bool Ctrl, bool Shift);
    void setGroupCondition(int skill_num, int condition_num,  bool state);
    void jump_to_bootloader();

    void setActiveL2Index  (int l2_index);
    void setActiveCondIndex(int    index);


    void redraw();
    void addL2Window(HWND hwnd);

    void loadConfig(QString file_name);
    void saveProject(QString file_name);
    void loadProject(QString file_name);
    void saveConfig(QString file_name);
    void addConfig(QString file_name);

    void resetL2Windows(QVector <HWND> *hwnd_list); //+


};


//Register classes to use in QVariant
Q_DECLARE_METATYPE(HWND)

#endif // MAINWINDOW_H
