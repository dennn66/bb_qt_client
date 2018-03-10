#include "telegrambot.h"
#include <QImage>

TelegramBot::TelegramBot(QString token, QObject *parent) : QObject(parent)
{
    chatID = 0;
    bSendScreenshot = false;
    iLastCP = 0;
    bPopupL2Window = false;
    sToken = token;

}

void TelegramBot::process(){
    bot = new Telegram::Bot(sToken, true, 500, 4);
    dtLastStatusReportTime = QDateTime::currentDateTime();
    dtNonezeroHPTime  = QDateTime::currentDateTime();
    connect(bot, SIGNAL(message(Telegram::Message)), this, SLOT(processMessage(Telegram::Message)), Qt::QueuedConnection);
    qDebug() << "Started Telegram Bot";
}

void TelegramBot::processMessage(Telegram::Message message)
{
    //qDebug() << "new message:" << message;

    if (bot && message.type == Telegram::Message::TextType) {
        QString command = message.string.toUpper();
        chatID = message.chat.id;
        if(command == "/OFF" || command == "0"|| command == "/0"){
            bot->sendMessage(chatID, "OK, stopping...");
            emit set_operation_state(false);
        } else if(command == "ON" || command == "1"|| command == "/1"){
            bot->sendMessage(chatID, "OK, starting...");
            bPopupL2Window = true;
            emit set_operation_state(true);
        } else if(command == "SHOW" || command == "2"|| command == "/2"){
            bot->sendMessage(chatID, "OK, sending scrinshot...");
            bSendScreenshot = true;
        } else  if(command == "POPUP" || command == "3"|| command == "/3"){
            bot->sendMessage(chatID, "OK, popup L2 window...");
            bPopupL2Window = true;
        } else if(command == "RESURRECTION" || command == "4"|| command == "/4"){
            bot->sendMessage(chatID, "OK, resurrecting...");
            bPopupL2Window = true;
            emit resurrect();
        } else if(command == "TEST" || command == "9"|| command == "/9"){
            bot->sendMessage(chatID, "OK, testing...");
            /*
            QImage* pattern = new QImage();
            QImage* source = new QImage();
            pattern->load("patterns\\r_target1.bmp");
            source->load("patterns\\r_target2.bmp");

            for(int w = 0; w < pattern->width();w++){
                for(int h = 0; h < pattern->height();h++){
                    QRgb frame_pix = pattern->pixel(QPoint(w, h));
                    QColor ct(frame_pix);
                    if(!(ct.red() == 255 && ct.green()==255 && ct.blue() == 255)){
                         QRgb image_pix = source->pixel(QPoint(w, h));
                         if (frame_pix != image_pix) pattern->setPixelColor(QPoint(w,h), QColor("#FFFFFFFF"));
                    }
                }
            }
            pattern->save("patterns\\r_pattern.bmp");
            */

        } else{
            bot->sendMessage(chatID, "Commands:\n /0 - OFF\n/1 - ON\n/2 - SHOW\n/3 - POPUP\n/4 - RESURRECTION");
        }
    }
}

void TelegramBot::sendTextMessage(QString text)
{
    bot->sendMessage(chatID, text);
}


void TelegramBot::showParserStatus(int updatetime, L2Window* l2w, QImage clicker_bk)
{
    Q_UNUSED(clicker_bk);
    Q_UNUSED(updatetime);
    if(chatID == 0) return;
    if(bSendScreenshot)
    {
        bSendScreenshot = false;
        l2w->getImage()->save("screenshot.bmp");
        QFile file("screenshot.bmp");
        if (file.open(QIODevice::ReadOnly))
        {
            bot->sendPhoto(chatID, &file);
        } else {
            qWarning() << "Error to opening file";
            bot->sendMessage(chatID, "Error To Opening File!");
        }
    }
    if(bPopupL2Window)
    {
        bPopupL2Window = false;
        emit popupL2Window(l2w->getHWND());
    }
}
