#ifndef HOTKEYS_H
#define HOTKEYS_H

#include <QObject>
#include <QSettings>
#include <QTextStream>

#include "hotkey.h"
#include "SystemKeyboardReadWrite.h"





class HotKeys : public QObject
{
    Q_OBJECT
public:
    explicit HotKeys(QObject *parent = 0);




private:
    QVector <HotKey*> hklist;
    bool bEnableHotKey;
    SystemKeyboardReadWrite *kb;
public slots:
   void process();
   void keyGlobalPressed(DWORD vkCode);
   void keyGlobalReleased(DWORD vkCode);

signals:
   void finished();
   void error(QString err);
   void setGroupState(int i, bool state);
   void toggleGroupState(int i);
   void set_operation_state(bool stt);
   void toggle_operation_state();
   void set_shift(bool bShift);
   void toggle_shift();

};

#endif // HOTKEYS_H
