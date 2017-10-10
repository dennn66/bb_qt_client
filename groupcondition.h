#ifndef GROUPCONDITION_H
#define GROUPCONDITION_H

#include <QObject>
#define GROUPSNUM 4

class GroupCondition : public QObject
{
    Q_OBJECT
public:
    explicit GroupCondition(QObject *parent = 0);
    void setGroupState(int group, bool state){group_condition[group] = state;}
    bool getGroupState(int group){return group_condition[group];}
private:
    bool group_condition[GROUPSNUM];

signals:

public slots:
};

#endif // GROUPCONDITION_H
