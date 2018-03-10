#include "eventprocessor.h"

EventProcessor::EventProcessor(bool bSound, QObject *parent) : QObject(parent)
{
    bEnableSound = bSound;
    iLastCP = 0;
    bDongleIsWorking = false;
    bNeedResurrection = false;

}

void EventProcessor::process()
{
    dtLastStatusReportTime = QDateTime::currentDateTime();
    dtNonezeroHPTime  = QDateTime::currentDateTime();
    qDebug() << "Started Event Processor";

}


void EventProcessor::showParserStatus(int updatetime, L2Window* l2w, QImage clicker_bk){
    Q_UNUSED(clicker_bk);
    Q_UNUSED(updatetime);
    static bool bModeCanged = false;
    QDateTime now = QDateTime::currentDateTime();
    //qDebug()<< "EventProcessor::showParserStatus timeout:" << dtLastStatusReportTime.secsTo(now);
    if(dtLastStatusReportTime.secsTo(now) > 60){
        //qDebug()<< "Timer over 60s: " << dtLastStatusReportTime.secsTo(now);
        int cp = l2w->getXP(idCP);
        if( (cp < 100) && (iLastCP - cp > 10)){
            iLastCP = cp;
            QString m;
            QTextStream m_str(&m);
            m_str << now.toString("hh:mm:ss") <<" CP going down: " << iLastCP << "%";
            emit sendTextMessage(m); //bot->sendMessage(chatID, m);
            if(bEnableSound)PlaySound((LPCWSTR)"sounds/cpgoingdown.wav", NULL, SND_FILENAME);
            //qDebug()<< "Play CP sound " << bEnableSound;
            dtLastStatusReportTime = now;
        } else if(iLastCP < cp) {
            iLastCP = cp;
        }

        if( (l2w->getXP(idHP) > 0) && (l2w->getXP(idHP) <= 100)){
                dtNonezeroHPTime  = now;
        } else {
            if(dtNonezeroHPTime.secsTo(now) > 5){
                if(bEnableSound)PlaySound((LPCWSTR)"sounds/dead.wav", NULL, SND_FILENAME);
                //qDebug()<< "Play HP sound " << bEnableSound;
                QString m;
                QTextStream m_str(&m);
                m_str << now.toString("hh:mm:ss") << " You are dead, CP: " << l2w->getXP(idCP) << "%";
                emit sendTextMessage(m);//bot->sendMessage(chatID, m);
                dtNonezeroHPTime = now.addYears(1);
                dtLastStatusReportTime = now;
            }
        }
    }

    if(bNeedResurrection){
        if( (l2w->getXP(idHP) > 0) && (l2w->getXP(idHP) <= 100)) bNeedResurrection = false;
    }
    if(l2w->is_res_avialable()){
        //GetCursorPos
        //The GetCursorPos function retrieves the cursor's position, in screen coordinates.

        //BOOL GetCursorPos(
        //LPPOINT lpPoint // cursor position
        //);
        POINT pt;
        if(GetCursorPos(&pt))
        {
            //qDebug() << "Mouse Cord: " << pt.x << ", " << pt.y;
            l2w->setMouseCoord(pt.x,pt.y);
        } else {
            qWarning() << "Mouse Cord call status: fail";
        }

        if(bDongleIsWorking && bNeedResurrection)
        {
            emit set_dongle_mode(true);
            bModeCanged = true;
            if(l2w->is_res_in_focus() )
            {
                emit set_operation_state(false);
                Sleep(60);
                emit set_mouse_report(0, 0, true, false, false);
                Sleep(60);
                emit set_mouse_report(0, 0, false, false, false);
                Sleep(60);
                emit set_dongle_mode(false);

            } else {
                emit set_mouse_report(l2w->get_res_DeltaX(), l2w->get_res_DeltaY(), false, false, false);
            }
        }

    } else {
        if(bModeCanged)
        {
            emit set_dongle_mode(false);
            bModeCanged = false;
        }
    }

}
