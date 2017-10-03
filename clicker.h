#ifndef CLICKER_H
#define CLICKER_H

#include <QWidget>
#include <QtGui>
#include <QVBoxLayout>

#include "l2collection\l2collection.h"
#include "l2collection\l2window\conditionmanager\keysettingsdialog.h"
#include "dongle\dongle.h"
#include "SystemMouseHook.h"

namespace Ui {
class Clicker;
}

class Clicker : public QWidget
{
    Q_OBJECT

public:
    explicit Clicker(L2Collection* l2c, QWidget *parent = 0);
    //Dongle* dongle;
    ~Clicker();
protected:

private:
    Ui::Clicker *ui;


   // bool group_enable[CONDBNUM];
    QCheckBox *keyenable2[GROUPSNUM];
    void toggleGroup(int group);
    void enableGroup(int group, bool state);
    bool isUnderWidget(QWidget* widget, int x, int y);
    SystemMouseHook *ms;
    int right_offset;
    int top_offset;
    QImage* green_frame;
    QImage* red_frame;
    bool bFindBarsIsPressed;
    bool bSettingsIsPressed;
    L2Collection* l2collection;


public slots:
    void cbDongleClicked(bool checked);
    void cbCtrlShiftClicked(bool checked);
    void cbKeyEnableBxClicked(bool checked);
    void showDongleStatus(unsigned char d_stt, unsigned char g_stt, int updatetime); /* */
    void showParserStatus(int updatetime);
    void isL2Active(bool isActive, int right, int top);
    // Broadcasts a key has been pressed
    void keyLPressed(int x, int y);
    // Broadcasts a key has been released
    void keyLReleased(int x, int y);


signals:
    void setGroupState(int i, bool state);//+
    void setDongleState(bool stt);//+
    void setModifiers(bool bCtrl, bool bShift);//+
    void findBars();//+
    void popupSettings();//+



};

#endif // CLICKER_H

