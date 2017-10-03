#ifndef L2COLLECTION_WORKER_H
#define L2COLLECTION_WORKER_H

#include <QObject>

class L2Collection_Worker : public QObject
{
    Q_OBJECT
public:
    explicit L2Collection_Worker(QObject *parent = 0);

signals:

public slots:
};

#endif // L2COLLECTION_WORKER_H