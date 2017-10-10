#ifndef CLICKER_H
#define CLICKER_H

#include <QWidget>
#include <QtGui>
#include <QVBoxLayout>

#include "keysettingsdialog.h"
#include "dongle.h"
#include "SystemMouseHook.h"
#include "l2window.h"


namespace Ui {
class Clicker;
}

class Clicker : public QWidget
{
    Q_OBJECT

public:
    explicit Clicker(QWidget *parent = 0);
    //Dongle* dongle;
    ~Clicker();
protected:

private:
    Ui::Clicker *ui;


   // bool group_enable[CONDBNUM];
    QLabel *keyenable1[GROUPSNUM];
    QCheckBox *keyenable2[GROUPSNUM];
    void toggleGroup(int group);
    void enableGroup(int group, bool state);
    bool isUnderWidget(QWidget* widget, int x, int y);
    static const char* StyleSheetCheckBox[5];
    SystemMouseHook *ms;
    int right_offset;
    int top_offset;
    QImage* green_frame;
    QImage* red_frame;
    bool bFindBarsIsPressed;
    bool bSettingsIsPressed;



public slots:
    void cbDongleClicked(bool checked);
    void cbCtrlShiftClicked(bool checked);
    void cbKeyEnableBxClicked(bool checked);
    void showDongleStatus(unsigned char d_stt, int updatetime); /* */
    void showParserStatus(int updatetime, L2Window* l2w, QImage clicker_bk);
    // Broadcasts a key has been pressed
    void keyLPressed(int x, int y);
    // Broadcasts a key has been released
    void keyLReleased(int x, int y);
    void updateGroupState(int num,  bool state);
    void set_visual_skill_state(int num, bool state, bool enable, bool groupstate);


signals:
    void set_operation_state(bool stt);
    void set_modifier(bool bCtrl, bool bShift);

    void setGroupState(int i, bool state);
//    void doSetState(bool stt);
//    void doSetModifier(bool bCtrl, bool bShift);
    void doActivateL2();
    void pbFindBarsClicked();
    void pbSettingsClicked();
};

#endif // CLICKER_H
