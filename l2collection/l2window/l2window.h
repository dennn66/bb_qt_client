#ifndef L2WINDOW_H
#define L2WINDOW_H

#include <QObject>

class L2Window : public QObject
{
    Q_OBJECT
public:
    explicit L2Window(QObject *parent = 0);

signals:

public slots:
};

#endif // L2WINDOW_H