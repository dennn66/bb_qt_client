#ifndef EVENTPROCESSOR_H
#define EVENTPROCESSOR_H

#include <QObject>
#include "l2window.h"
#include "dongle.h"

class EventProcessor : public QObject
{
    Q_OBJECT
public:
    explicit EventProcessor(bool bSound, QObject *parent = nullptr);
private:
    QDateTime dtLastStatusReportTime;
    QDateTime dtNonezeroHPTime;
    bool bEnableSound;
    int iLastCP;
    bool bDongleIsWorking;
    bool bNeedResurrection;

signals:
    void finished();
    void error(QString err);
    void set_dongle_mode(bool bMode);
    int set_mouse_report(qint8 x, qint8 y, bool left, bool right, bool middle);
    void sendTextMessage(QString text);
    void set_operation_state(bool stt);



public slots:
    void process();
    void showParserStatus(int updatetime, L2Window* l2w, QImage clicker_bk);
    void showDongleStatus(unsigned char d_stt,  int updatetime)
    {
        Q_UNUSED(updatetime);
        bDongleIsWorking = ((d_stt & (1 << DEVICE_STATUS)) > 0);

    }
    void resurrect(){bNeedResurrection = true;}

};

#endif // EVENTPROCESSOR_H
