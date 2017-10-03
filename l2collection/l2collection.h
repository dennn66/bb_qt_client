#ifndef L2COLLECTION_H
#define L2COLLECTION_H

#include <QObject>
#include "l2parser\l2window\l2window.h"
#include "l2parser\l2window\l2parser.h"
#include "boredombreaker.h"




BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lParam);

class L2Collection : public QObject
{
    Q_OBJECT
public:
    explicit L2Collection(QObject *parent = 0);

private:
    QThread* l2_parser_thread;
    L2parser* l2_parser;
    QVector <L2Window*> l2list;

    BoredomBreaker* gui;
    void enumerateL2();
    bool isValidIndex(int index);
    void addL2Window(HWND hwnd);

signals:

public slots:
    void doActivateL2();
};

#endif // L2COLLECTION_H
