#ifndef L2COLLECTION_H
#define L2COLLECTION_H

#include <QObject>

class L2Collection : public QObject
{
    Q_OBJECT
public:
    explicit L2Collection(QObject *parent = 0);

signals:

public slots:
};

#endif // L2COLLECTION_H