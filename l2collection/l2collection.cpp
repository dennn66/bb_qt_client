#include "l2collection.h"

L2Collection::L2Collection(QObject *parent) : QObject(parent)
{
    //LOAD CONFIG BB.ini
    QSettings sett("bb.ini", QSettings::IniFormat);
    activeIndex = -1;

    default_file_name = sett.value("MAIN/DefaultProject", "default.bbproj").toString();




}

bool L2Collection::isValidIndex(int index){
    if((index == -1)||(l2list.isEmpty())||(index >= l2list.size()))return false;
    return true;
}

/*
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

*/


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
    if(activeIndex > l2list.size()-1) activeIndex = l2list.size()-1;

    l2w->conditionmanager->LoadProject(default_file_name);
}

void L2Collection::spinonce()
{
    qDebug("L2Collection::spinonce() Thread: %d", (int) QThread::currentThreadId());
    if(currentl2w() != NULL) {
        try {
            currentl2w()->l2parser->check();
        } catch(...) {
            qDebug("Check failed");
        }
    }
}


void L2Collection::setActiveSettings(int index)
{
    qDebug("L2parser::setActiveL2W");
    if(!isValidIndex(activeIndex)){ activeIndex = 0xFF; return;}
    if(!isValidConditionIndex(index)){ return;}
    activateSettings(index);
    //update settings view
    emit resetSetView();

}

void L2Collection::setActiveL2W(int index)
{
    qDebug("L2parser::setActiveL2W");
    if(isValidIndex(index)) activeIndex = index;

    //update windows view
    emit resetWinView();
    //emit resetSetView();
}


void L2Collection::findBars(){
    if(!isValidIndex(activeIndex)) return;
    l2list[activeIndex]->l2parser->resetBars();
    l2list[activeIndex]->l2parser->resetSkillbar();
}



void L2Collection::setNic(QString text){
    if(!isValidIndex(activeIndex)) return;
    l2list[activeIndex]->conditionmanager->getCurrentSettings()->nic = text;
}

void L2Collection::addConfig(QString file_name){
    if(!isValidIndex(activeIndex)) return;
    l2list[activeIndex]->conditionmanager->AddConfig(file_name);
    emit doSaveAllToDongle();
    emit resetSetView();
}

void L2Collection::loadConfig(QString file_name){
    if(!isValidIndex(activeIndex)) return;
    l2list[activeIndex]->conditionmanager->loadConfig(file_name);
    emit resetSetView();
    emit doSaveAllToDongle();
}


void L2Collection::saveConfig(QString file_name){
    if(!isValidIndex(activeIndex)) return;
    l2list[activeIndex]->conditionmanager->saveConfig(file_name);
}

void L2Collection:: loadProject(QString file_name){
    if(!isValidIndex(activeIndex)) return;
    l2list[activeIndex]->conditionmanager->LoadProject(file_name);
    emit resetSetView();
    emit doSaveAllToDongle();
}



void L2Collection::saveProject(QString file_name){
    if(!isValidIndex(activeIndex)) return;
    l2list[activeIndex]->conditionmanager->saveProject(file_name);
}


void L2Collection::setConditionState(int, bool){
        //fix
        emit resetKeyView(); //+
}


void L2Collection::editCondition(int condnum){
    if(!isValidIndex(activeIndex)){ activeIndex = 0xFF; return;}
    if(!isValidConditionIndex(condnum)){ return;}
    KeyCondition cond(".");
    //cond = *(l2list[activeIndex]->conditionmanager->getCurrentSettings()->condition[condnum]);
/*
    KeySettingsDialog dlg(&cond, l2list[activeIndex], condnum);
    if(dlg.exec() == QDialog::Accepted){
      *(l2list[activeIndex]->conditionmanager->getCurrentSettings()->condition[condnum]) = cond;
    }
*/
    emit doSendKeyToDongle(activeIndex);
    emit resetKeyView(); //+

}



void L2Collection::resetL2Windows(QVector <HWND> *hwnd_list){
    qDebug("L2Collection::::resetL2Windows(QVector <HWND> hwnd_list)");
    QVector <L2Window*> l2temp;
    QVector <L2Window*> l2old;
    qDebug("copy  l2old = l2list");
    l2old = l2list;
    qDebug("sizes  l2old=%d l2list=%d", l2old.size(), l2list.size());

    L2Window* active = currentl2w();
    qDebug("L2Window* active = currentl2w() %d", (int)active);

    qDebug("Start regrouping");

    //Add all  windows
    for(int hwndindex = 0; hwndindex < hwnd_list->size(); hwndindex++){
        bool found = false;
        //Try to find existing windows
        for(int l2index = 0; l2index<l2old.size(); l2index++){
            if(l2old[l2index]->getHWND() == (*hwnd_list)[hwndindex]){
                qDebug("Found hwnd %d", (int)(*hwnd_list)[hwndindex]);
                l2temp.append(l2old[l2index]);
                qDebug("l2temp.append size %d", l2temp.size());
                l2old.remove(l2index);
                qDebug("l2old.remove size %d", l2old.size());
                found = true;
                break;
            }
        }
        // Add new windows
        if(!found){
            qDebug("Not found hwnd %d", (int)(*hwnd_list)[hwndindex]);
            L2Window* l2w = new L2Window((*hwnd_list)[hwndindex]);
            if(l2w != NULL){
                l2temp.append(l2w);
                qDebug("l2temp.append size %d", l2temp.size());
            } else {
                qDebug("l2w creation failed");
            }
        }
    }

    // Copy new list to permanent location and set new active window
    int newIndex = 0;
    for(int l2index = 0; l2index<l2temp.size(); l2index++){
        if(l2temp[l2index] == active){
            newIndex = l2index;
            qDebug("set activeIndex %d", newIndex);
            break;
        }
    }
    activeIndex = 0xFF;
    l2list.clear();
    if(l2temp.size()>0){
        l2list = l2temp;
        setActiveL2W(newIndex);
    }

    //Remove all closed windows
    qDebug("need to remove %d", l2old.size());
    for(int l2index = 0; l2index<l2old.size(); l2index++){
        delete l2old[l2index];
        qDebug("delete %d", l2index);
        l2old.remove(l2index);
        qDebug("remove %d", l2index);
    }

    qDebug("resetL2Windows done, l2list size %d",  l2list.size());
}
