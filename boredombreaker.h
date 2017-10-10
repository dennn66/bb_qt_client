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
#include "SystemKeyboardReadWrite.h"

#include "l2window.h"
#include "dongle.h"
#include "l2parser.h"
#include "keysettingsdialog.h"
#include "clicker.h"
#include "hotkeys.h"



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
    void addL2Window(HWND hwnd);
private:
    Ui::BoredomBreaker *ui;
    QVector <L2Window*> l2list;

    QLabel *keylabel[KEYNUM];
    QCheckBox *keyenable[KEYNUM];
    QCheckBox *keyenable2[GROUPSNUM];
    QPushButton *keysettings[KEYNUM];

    Dongle* dongle;
    Clicker* clicker;
    L2parser* l2_parser;


    QProgressBar *pb[BARNUM];
    SystemKeyboardReadWrite *kb;


    HotKeys* hk;

    bool bEnableSound;

    QString default_file_name;

    int ellipsed_time;
    static const char* StyleSheet[BARNUM+1];
    static const char* StyleSheetCheckBox[5];
    static const char* StyleSheetLabel[6];

    void enumerateL2();
    bool isValidIndex(int index);
    void emitKeySetup(int l2index, int key_index);


public slots:
    void showDongleStatus(unsigned char d_stt, int updatetime); /* */
    void showParserStatus(int updatetime, L2Window* l2w, QImage clicker_bk);
    void cbDongleClicked(bool checked);
    void cbCtrlShiftClicked(bool checked);
    void cbKeyEnableClicked(bool checked);
    void cbKeyEnableBxClicked(bool checked);
    void pbLoadClicked();
    void pbSaveProjectClicked();
    void pbLoadProjectClicked();
    void pbSaveClicked();
    void pbAddClicked();
    void pbToDongleClicked();
    void pbJumpToBootloaderClicked();
    void pbKeySettingsClicked();
    void pbEnumerateClicked();
    void pbFindBarsClicked();
    void cmbWinListActivated(int index);
    void cmbCondSetListActivated(int index);
    void cmbCondSetListTextChanged(const QString &text);
    void keyGlobalPressed(DWORD vkCode);
    void keyGlobalReleased(DWORD vkCode);
    void doActivateL2();
    void pbSettingsClicked();
    void updateGroupState(int num,  bool state);
    void set_visual_skill_state(int num, bool state, bool enable, bool groupstate);


signals:
    void setGroupState(int i, bool state);

    void set_operation_state(bool stt);
    void set_modifier(bool bCtrl, bool bShift);
    void setup_key(int index,  bool state, unsigned char Key, float PauseTime, float ReleaseTime, float ConditionTime,  bool Ctrl, bool Shift);
    void setGroupCondition(int skill_num, int condition_num,  bool state);
    void jump_to_bootloader();
    void setActiveL2W(L2Window* l2w);
    void redraw();

    //void set_mode(bool bMode);
};

#endif // MAINWINDOW_H
