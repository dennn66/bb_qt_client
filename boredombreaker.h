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

#include "l2collection\l2collection_worker.h"


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
    void addL2HWND(HWND hwnd);

private:
    QVector <HWND> hwnd_list;

    Ui::BoredomBreaker *ui;

    QLabel *keylabel[KEYNUM];
    QCheckBox *keyenable[KEYNUM];
    QCheckBox *keyenable2[GROUPSNUM];
    QPushButton *keysettings[KEYNUM];
    QProgressBar *pb[BARNUM];

    int ellipsed_time;
    static const char* StyleSheet[BARNUM+1];
    static const char* StyleSheetCheckBox[5];
    static const char* StyleSheetLabel[2];

    L2Collection_Worker* l2collection_worker;
    L2Collection* l2collection;

    Clicker* clicker;

    QString default_file_name;

    SystemKeyboardReadWrite *kb;
    HotKeys* hk;

    bool bEnableSound;
//    bool bEnableModifier;



    void enumerateL2();
    void startL2enumerating();



public slots:
    void showDongleStatus(unsigned char d_stt, unsigned char g_stt, int updatetime); /* */
    void showParserStatus(int updatetime);

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

    void popupWindow();

    void resetL2WindowsCombo(); //+
    void resetSettingsCombo();  //+
    void resetKeys();  //+


signals:
    //void setDongleState(int stt);


    void resetL2Windows(QVector <HWND> *hwnd_list); //+

    void setDongleState(bool);
    void setModifiers(bool, bool);
    void saveAllToDongle();
    void rebootDongle();

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

    void setGroupState(int, bool);

};

#endif // MAINWINDOW_H
