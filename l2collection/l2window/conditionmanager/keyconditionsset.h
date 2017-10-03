#ifndef KEYCONDITIONSSET_H
#define KEYCONDITIONSSET_H

#include <QString>
#include <QTextStream>
#include <QSettings>
#include "l2collection\l2window\l2parser\objects\xpbar.h"
#include "keycondition.h"

#define KEYNUM 48

class KeyConditionsSet: public QObject
{
    Q_OBJECT

public:
    explicit KeyConditionsSet(QObject *parent = 0);
    ~KeyConditionsSet();
    KeyCondition* condition[KEYNUM];

    QString settings_file_name;
    QString nic;

    int LoadConfig(QString file_name);
    int SaveConfig(QString file_name);
private:
    static const char* DefaultKeyDB[KEYNUM];

signals:

public slots:
};

#endif // KEYCONDITIONSSET_H
