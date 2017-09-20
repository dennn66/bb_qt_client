#include "SystemMouseHook.h"

SystemMouseHook::SystemMouseHook() :
    QObject()
{
    // Assign to null
    mouseHook = NULL;
}


LRESULT CALLBACK SystemMouseHook::mouseProcedure(int nCode, WPARAM wParam, LPARAM lParam)
{
    qDebug("mouseProcedure");

    Q_UNUSED(nCode);
    MOUSEHOOKSTRUCT * pMouseStruct = (MOUSEHOOKSTRUCT *)lParam;


    if(pMouseStruct != nullptr) {
        qDebug("pMouseStruct OK");
//pMouseStruct->pt.x
        switch (wParam) {
        case WM_LBUTTONDOWN:
            qDebug("keyLPressed");
            emit SystemMouseHook::instance()->keyLPressed(pMouseStruct->pt.x, pMouseStruct->pt.y);
            break;
        case WM_LBUTTONUP:
            qDebug("keyLReleased");
            emit SystemMouseHook::instance()->keyLReleased(pMouseStruct->pt.x, pMouseStruct->pt.y);
            break;
        default:
            break;
        }
        instance()->setConnected( false );
        instance()->setConnected( true );
    }

    if(false){

        QFile* testFile = new QFile("Timing.csv");
        QTextStream* fStream;
        if(testFile->open(QFile::WriteOnly| QIODevice::Append)){
            fStream = new QTextStream(testFile);
            #ifdef Q_WS_WIN
                fStream->setCodec("Windows-1251");
            // Под остальными ОС - utf8
            #else
                fStream->setCodec("utf-8");
            #endif

            if(fStream && fStream->device()){
                *fStream << QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz") << ";" << wParam << ";" << QDateTime::currentDateTime().toMSecsSinceEpoch()  << "\r\n";

            }
            testFile->close();
        }

     }

    //return CallNextHookEx(NULL, nCode, wParam, lParam);
    return false;

}

bool SystemMouseHook::connected()
{
    return mouseHook;
}

bool SystemMouseHook::setConnected(bool state)
{
    if(state && mouseHook == NULL)
    {
        try {
            mouseHook = SetWindowsHookEx(WH_MOUSE_LL, mouseProcedure, GetModuleHandle(NULL), 0);

        } catch(...) {
            ;;
            qDebug("SystemMouseHook::SetWindowsHookEx failed\n");

        }

        return mouseHook;
    }
    else
    {

        try {
            UnhookWindowsHookEx(mouseHook);
        } catch(...) {
            ;;
            qDebug("SystemMouseHook::UnhookWindowsHookEx(keyboardHook) failed\n");

        }
        mouseHook = NULL;

        return mouseHook;
    }
}

SystemMouseHook* SystemMouseHook::instance()
{
    static SystemMouseHook* pMouseHookInstance = new SystemMouseHook();
    qDebug("SystemMouseHook:::instance() failed\n");

    return pMouseHookInstance;
}
