#include "l2collection.h"

L2Collection::L2Collection(QObject *parent) : QObject(parent)
{
    l2_parser_thread = new QThread;
    l2_parser = new L2parser();
    l2_parser->moveToThread(l2_parser_thread);

    connect(l2_parser_thread, SIGNAL(started()), l2_parser, SLOT(process()));
    connect(l2_parser, SIGNAL(finished()), l2_parser_thread, SLOT(quit()));
    connect(l2_parser, SIGNAL(finished()), l2_parser, SLOT(deleteLater()));
    connect(l2_parser_thread, SIGNAL(finished()), l2_parser_thread, SLOT(deleteLater()));


    l2_parser_thread->start();

    connect(gui, SIGNAL(setActiveL2W(L2Window*)), l2_parser, SLOT(setActiveL2W(L2Window*)));

    connect(gui, SIGNAL(doActivateL2()), l2_parser, SLOT(doActivateL2()));
    connect(gui, SIGNAL(doActivateL2()), this, SLOT(doActivateL2()));
    connect(l2_parser, SIGNAL(isL2Active(bool, int, int)), gui, SLOT(isL2Active(bool, int, int)));
    connect(l2_parser, SIGNAL(showParserStatus(int, L2Window*)), gui, SLOT(showParserStatus(int, L2Window*)));


    enumerateL2();
}

bool L2Collection::isValidIndex(int index){
    if((index == -1)||(l2list.isEmpty())||(index >= l2list.size()))return false;
    return true;
}

void L2Collection::doActivateL2()
{
    qDebug("BoredomBreaker::doActivateL2W()");

    int index = ui->cmbWinList->currentIndex();
    if(!isValidIndex(index))return;

    HWND m_hWnd = (HWND)l2list[index]->getHWND();
    HWND hCurWnd = ::GetForegroundWindow();
    DWORD dwMyID = ::GetCurrentThreadId();
    DWORD dwCurID = ::GetWindowThreadProcessId(hCurWnd, NULL);
    ::AttachThreadInput(dwCurID, dwMyID, TRUE);
    ::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    ::SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    ::SetForegroundWindow(m_hWnd);
    ::AttachThreadInput(dwCurID, dwMyID, FALSE);
    ::SetFocus(m_hWnd);
    ::SetActiveWindow(m_hWnd);

}


void L2Collection::enumerateL2(){
    qDebug("BoredomBreaker::enumerateL2()");
    if((BOOL)EnumWindows(&EnumWindowsProc,reinterpret_cast<long int>(this)))
    {
            qDebug("EnumWindows - ok");
    }else{qDebug("EnumWindows - not ok");}

}

void L2Collection::addL2Window(HWND hwnd){
    qDebug("BoredomBreaker::addL2Window(HWND hwnd): %d", (int) hwnd);
    if(!l2list.isEmpty())
    {
        for(int index = 0; index < l2list.size(); index++){
            if(l2list[index]->getHWND() == hwnd) return;
        }
    }
    L2Window *l2w = new L2Window(hwnd);
    l2list.append(l2w);
    ui->cmbWinList->addItem(l2w->getTitle());

    l2w->LoadProject(default_file_name);

    cmbWinListActivated(ui->cmbWinList->currentIndex());
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lParam)
{
        qDebug("BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lParam)");
        char szTextWin[255];DWORD dwPID = (DWORD) NULL;
        GetWindowTextA(hwnd,szTextWin,sizeof(szTextWin));
        if(strstr(szTextWin, "Lineage") != 0){
            qDebug(" - Txt: %s", szTextWin);
            qDebug(" - TID: %d", (int) GetWindowThreadProcessId(hwnd,&dwPID));
            qDebug(" - PID: %d", (int) dwPID);
            qDebug("  HWND: %d", (int) hwnd);
             //Capture(hwnd);
            BoredomBreaker * bb = reinterpret_cast<BoredomBreaker*>(lParam);
            bb->addL2Window(hwnd);
        }
        return TRUE;
}
