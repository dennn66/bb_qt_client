#ifndef TELEGRAMBOT_H
#define TELEGRAMBOT_H


#include <QObject>
#include "qttelegrambot.h"
#include "l2window.h"
#include <QDateTime>


class TelegramBot : public QObject
{
    Q_OBJECT
public:
    explicit TelegramBot(QString token, QObject *parent = nullptr);
    Telegram::Bot *bot;
    qint32 chatID;
    QDateTime dtLastStatusReportTime;
    QDateTime dtNonezeroHPTime;
    bool bSendScreenshot;
    bool bPopupL2Window;
    int iLastCP;
    QString sToken;

signals:
    void set_operation_state(bool stt);
    void finished();
    void error(QString err);
    void popupL2Window(HWND hwnd);
    void resurrect();



public slots:
    void process();
    void processMessage(Telegram::Message message);
    void showParserStatus(int updatetime, L2Window* l2w, QImage clicker_bk);
    void sendTextMessage(QString text);


};

#endif // TELEGRAMBOT_H
