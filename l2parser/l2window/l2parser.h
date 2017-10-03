#ifndef L2PARSER_H
#define L2PARSER_H

#include <QObject>
#include <QTime>

#include "l2window.h"

class L2parser : public QObject
{
    Q_OBJECT
public:
    L2Window* currentl2w;

    explicit L2parser(QObject *parent = 0);



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

#endif // L2PARSER_H
