#ifndef L2COLLECTION_WORKER_H
#define L2COLLECTION_WORKER_H

#include <QObject>
#include <QTime>

#include "l2collection.h"

class L2Collection_Worker : public QObject
{
    Q_OBJECT
public:
     L2Collection_Worker();
     ~L2Collection_Worker();
     L2Collection* l2collection;

signals:
     void finished();
     void error(QString err);
     void turn_completed(bool result, int ellipsedtime);

public slots:
    void process();
private:
};

#endif // L2COLLECTION_WORKER_H

