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
   void setActiveL2W(L2Window* l2w);
   void setGroupState(int num,  bool state);
   void redraw();

signals:
   void finished();
   void error(QString err);
   void showParserStatus(int updatetime, L2Window* l2w, QImage clicker_bk);
//   void isL2Active(bool isActive);
   //void set_condition_state(int num, bool state);
//   void set_l2_skill_state(int num, bool state);
//   void setGroupCondition(int skill_num, int condition_num,  bool state);

   void set_dongle_skill_state(int num, bool state);
   void set_visual_skill_state(int num, bool state, bool enable, bool groupstate);
   void updateGroupState(int num,  bool state);

};

#endif // L2PARSER_H
