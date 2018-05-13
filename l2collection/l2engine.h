#ifndef L2ENGINE_H
#define L2ENGINE_H

#include <QObject>
#include <QTime>

#include "l2collection\l2collection.h"

class L2engine : public QObject
{
    Q_OBJECT
public:
    L2Window* currentl2w;

    explicit L2engine(QObject *parent = 0);



public slots:
   void process();
   void doActivateL2();
   void setActiveL2W(L2Window* l2w);

//   void setGroupState(int i, bool state);



signals:
   void finished();
   void error(QString err);
   void showParserStatus(int updatetime, L2Window* l2w);
   void isL2Active(bool isActive, int right, int top);


};

#endif // L2ENGINE_H
