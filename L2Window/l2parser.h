#ifndef L2PARSER_H
#define L2PARSER_H

#include <QObject>
#include <QTime>

#include "l2window.h"

#include "groupmanager.h"
#include "conditionmgr.h"

#include "window.h"
#include "elements/skillbar.h"
#include "keysettingsdialog.h"

Q_DECLARE_METATYPE(QVector <HWND>)
Q_DECLARE_METATYPE(QVector <QString>)


class L2parser : public QObject
{
    Q_OBJECT
public:

    explicit L2parser(QObject *parent = 0);

    void emitKeySetup(int key_index);
private:
    QVector <L2Window*> l2list;
    int currentl2i;

    QString default_file_name;

    void doUpdateConditiosList();
    void doUpdateL2WindowsList();
    L2Window * getCurrentL2W();
    bool isValidL2W();
    bool bDongleIsWorking;



public slots:
   void process();
   void setGroupState(int num,  bool state);
   void toggleGroupState(int i);
   void redraw();
   void toggleRuleState(int key_index);
   void editRule(int key_index);
   void changeNic(const QString &text);
   void send_all_keys_to_dongle();
   void resetBars();
   void setActiveL2Index  (int l2_index);
   void setActiveCondIndex(int    index);
   void loadConfig(QString file_name);
   void saveProject(QString file_name);
   void loadProject(QString file_name);
   void saveConfig(QString file_name);
   void addConfig(QString file_name);

   void resetL2Windows(QVector <HWND> *hwnd_list); //+
   void showDongleStatus(unsigned char d_stt, int updatetime); /* */



signals:
   void finished();
   void error(QString err);
   void showParserStatus(int updatetime, L2Window* l2w, QImage clicker_bk);

   void updateConditiosList(QVector <QString> list, int current_index, QString file_name);
   void updateL2WindowsList(QVector <QString> list, int current_index, QString file_name);

   void set_dongle_skill_state(int num, bool state);

   void updateGroupState(int num,  bool state);
   void setup_key(int index,  bool state, unsigned char Key, float PauseTime, float ReleaseTime, float ConditionTime,  bool Ctrl, bool Shift);
   void setActiveL2W(L2Window* l2w, int l2_index);

};

#endif // L2PARSER_H
