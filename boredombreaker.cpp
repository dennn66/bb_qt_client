//
#include "boredombreaker.h"
#include "ui_boredombreaker.h"


const char* BoredomBreaker::StyleSheet[BARNUM+1] = {
    "QProgressBar::chunk {background: #805300 ;}",
    "QProgressBar::chunk {background: #881D18 ;}",
    "QProgressBar::chunk {background: #03327E ;}",
    "QProgressBar::chunk {background: #318A10 ;}",
   "QProgressBar::chunk {background: #881D18 ;}",
   "QProgressBar::chunk {background: #03327E ;}",
    "QProgressBar::chunk {background: #881D18 ;}",
    "QProgressBar::chunk {background: #881D18 ;}",
    "QProgressBar::chunk {background: #7F7F7F ;}"
};

const char* BoredomBreaker::StyleSheetCheckBox[5] = {
    "QCheckBox::indicator  {background: #805300 ;}",  //YELLOW
    "QCheckBox::indicator  {background: #881D18 ;}",  //RED
    "QCheckBox::indicator  {background: #03327E ;}",  //BLUE
    "QCheckBox::indicator  {background: #318A10 ;}",  //GREEN
    "QCheckBox::indicator  {background: #7F7F7F ;}"   //GREY
};

const char* BoredomBreaker::StyleSheetLabel[2] = {
    "QLabel  {background: #805300 ;}",  //YELLOW
    "QLabel  {background: #7f7f7f ;}"  //GREY
};

BoredomBreaker::BoredomBreaker(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BoredomBreaker)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    ellipsed_time = 0;
    pb[idCP] = ui->barCP;
    pb[idHP] = ui->barHP;
    pb[idMP] = ui->barMP;
    pb[idVP] = ui->barVP;
    pb[idMobHP] = ui->barMobHP;
    pb[idMobMP] = ui->barMobMP;
    pb[idPet1HP] = ui->barPet1HP;
    pb[idPet2HP] = ui->barPet2HP;

    //LOAD CONFIG BB.ini
    QSettings sett("bb.ini", QSettings::IniFormat);

    default_file_name = sett.value("MAIN/DefaultProject", "default.bbproj").toString();

    bEnableSound = sett.value("MAIN/EnableSound", 1).toBool();

    bool bEnableHotKey = sett.value("MAIN/EnableHotKey", 0).toBool();

    for(int j = idCP; j < BARNUM; j++ ){
        pb[j]->setMaximum(100);
        pb[j]->setMinimum(0);
        pb[j]->setStyleSheet(StyleSheet[BARNUM]);
        pb[j]->setValue(100);
        pb[j]->setVisible(true);
    }

    QGridLayout *layout = new QGridLayout;
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<12;j++)
        {
            int index = (3-i)*12+j;
            QGridLayout *sell  = new QGridLayout;
            QGridLayout *halfsell  = new QGridLayout;
            keylabel[index] = new QLabel("");
            keyenable[index] = new QCheckBox("");
            keysettings[index] = new QPushButton("");
            halfsell->addWidget(keyenable[index],0, 0);
            halfsell->addWidget(keysettings[index],0, 1);
            keyenable[index]->setChecked (false);
            sell->addWidget(keylabel[index],0, 0);
            sell->addLayout(halfsell,1, 0);
            layout->addLayout(sell,i, j);
        }
    }
    ui->chkbox_widget->setLayout(layout);


    QGridLayout *layout_2 = new QGridLayout;
    QString key_label = "B";
    QTextStream key_label_stream(&key_label);

    for(int i=0;i<GROUPSNUM;i++)
    {
        int j=0;
        key_label = "B";
        key_label_stream <<  i+1;
        QGridLayout *sell  = new QGridLayout;
        keyenable2[i] = new QCheckBox(key_label.toStdString().c_str());
        sell->addWidget(keyenable2[i],0, 0);
        keyenable2[i]->setChecked (1);
        layout_2->addLayout(sell,i, j);
    }
    ui->chkbox_widget_2->setLayout(layout_2);

    connect(ui->cbDongle, SIGNAL(clicked(bool)), SLOT(cbDongleClicked(bool)));
    connect(ui->cbCtrl, SIGNAL(clicked(bool)), SLOT(cbCtrlShiftClicked(bool)));
    connect(ui->cbShift, SIGNAL(clicked(bool)), SLOT(cbCtrlShiftClicked(bool)));
    connect(ui->pbLoadProject, SIGNAL(clicked()), SLOT(pbLoadProjectClicked()));
    connect(ui->pbSaveProject, SIGNAL(clicked()), SLOT(pbSaveProjectClicked()));
    connect(ui->pbLoad, SIGNAL(clicked()), SLOT(pbLoadClicked()));
    connect(ui->pbSave, SIGNAL(clicked()), SLOT(pbSaveClicked()));
    connect(ui->pbAdd, SIGNAL(clicked()), SLOT(pbAddClicked()));
    connect(ui->pbDongle, SIGNAL(clicked()), SLOT(pbToDongleClicked()));
    connect(ui->pbJumpToBootloader, SIGNAL(clicked()), SLOT(pbJumpToBootloaderClicked()));
    connect(ui->pbFindBars, SIGNAL(clicked()), SLOT(pbFindBarsClicked()));
    connect(ui->pbEnumerate, SIGNAL(clicked()), SLOT(pbEnumerateClicked()));
    connect(ui->cmbWinList, SIGNAL(activated(int)), SLOT(cmbWinListActivated(int)));
    connect(ui->cmbCondSetList, SIGNAL(activated(int)), SLOT(cmbCondSetListActivated(int)));
    connect(ui->leNic, SIGNAL(textChanged(const QString &)), SLOT(cmbCondSetListTextChanged(const QString &)));

    for(int i = 0; i< 48; i++){
        connect(keyenable[i], SIGNAL(clicked(bool)), SLOT(cbKeyEnableClicked(bool)));
        connect(keysettings[i], SIGNAL(clicked()), SLOT(pbKeySettingsClicked()));
    }
    for(int i = 0; i< GROUPSNUM; i++){
        connect(keyenable2[i], SIGNAL(clicked(bool)), SLOT(cbKeyEnableBxClicked(bool)));
    }

    //dongle_thread = new QThread;
    dongle_worker = new DongleWorker();
    //dongle_worker->moveToThread(dongle_thread);

    //connect(dongle_thread, SIGNAL(started()), dongle_worker, SLOT(process()));
    //connect(dongle_worker, SIGNAL(finished()), dongle_thread, SLOT(quit()));
    //connect(dongle_worker, SIGNAL(finished()), dongle_worker, SLOT(deleteLater()));
    //connect(dongle_thread, SIGNAL(finished()), dongle_thread, SLOT(deleteLater()));

    l2_parser_thread = new QThread;
    l2_parser = new L2parser();
    l2_parser->moveToThread(l2_parser_thread);

    connect(l2_parser_thread, SIGNAL(started()), l2_parser, SLOT(process()));
    connect(l2_parser, SIGNAL(finished()), l2_parser_thread, SLOT(quit()));
    connect(l2_parser, SIGNAL(finished()), l2_parser, SLOT(deleteLater()));
    connect(l2_parser_thread, SIGNAL(finished()), l2_parser_thread, SLOT(deleteLater()));


    l2_parser_thread->start();

    if(bEnableHotKey){
        kb = SystemKeyboardReadWrite::instance();
        kb->setConnected(true);
        connect(kb, SIGNAL(keyPressed(DWORD )), SLOT(keyGlobalPressed(DWORD)));
        connect(kb, SIGNAL(keyReleased(DWORD)), SLOT(keyGlobalReleased(DWORD)));
    }
    connect(dongle_worker, SIGNAL(showDongleStatusSig(unsigned char, unsigned char,int)), this,    SLOT(showDongleStatus(unsigned char, unsigned char, int)));

    connect(this, SIGNAL(doSendKeyToDongle(int)), dongle_worker, SLOT(doSendKeyToDongle(int)));
    connect(this, SIGNAL(doSetState(bool)), dongle_worker, SLOT(doSetState(bool)));
    connect(this, SIGNAL(doSetModifier(bool, bool)), dongle_worker, SLOT(doSetModifier(bool, bool)));
    connect(this, SIGNAL(doSaveAllToDongle()), dongle_worker, SLOT(doSaveAllToDongle()));
    connect(this, SIGNAL(doJumpToBootloader()), dongle_worker, SLOT(doJumpToBootloader()));

    connect(this, SIGNAL(setActiveL2W(L2Window*)), dongle_worker, SLOT(setActiveL2W(L2Window*)));
    connect(this, SIGNAL(setActiveL2W(L2Window*)), l2_parser, SLOT(setActiveL2W(L2Window*)));


    clicker = new Clicker;
    connect(clicker, SIGNAL(setDongleGroupState(int, bool)), dongle_worker, SLOT(setGroupState(int, bool)));
    connect(this, SIGNAL(setDongleGroupState(int, bool)), dongle_worker, SLOT(setGroupState(int, bool)));

    connect(clicker, SIGNAL(doSetState(bool)), dongle_worker, SLOT(doSetState(bool)));
    connect(clicker, SIGNAL(doSetModifier(bool, bool)), dongle_worker, SLOT(doSetModifier(bool, bool)));
    connect(clicker, SIGNAL(doActivateL2()), l2_parser, SLOT(doActivateL2()));
    connect(clicker, SIGNAL(doActivateL2()), this, SLOT(doActivateL2()));
    connect(l2_parser, SIGNAL(isL2Active(bool, int, int)), clicker, SLOT(isL2Active(bool, int, int)));
    connect(l2_parser, SIGNAL(showParserStatus(int, L2Window*)), this, SLOT(showParserStatus(int, L2Window*)));
    connect(l2_parser, SIGNAL(showParserStatus(int, L2Window*)), clicker, SLOT(showParserStatus(int, L2Window*)));
    connect(clicker, SIGNAL(pbFindBarsClicked()), this, SLOT(pbFindBarsClicked()));
    connect(clicker, SIGNAL(pbSettingsClicked()), this, SLOT(pbSettingsClicked()));
    connect(dongle_worker, SIGNAL(showDongleStatusSig(unsigned char, unsigned char,int)), clicker, SLOT(showDongleStatus(unsigned char, unsigned char, int)));

    enumerateL2();

    //dongle_thread->start();
    qDebug("BoredomBreaker start play");

    if(bEnableSound) QSound::play("sounds/on.wav");

    qDebug("BoredomBreaker stop play");

    hk = new HotKeys(this);

}


BoredomBreaker::~BoredomBreaker()
{
    qDebug("BoredomBreaker::~BoredomBreaker");
    delete ui;
    delete clicker;
}

void BoredomBreaker::pbSettingsClicked()
{
    qDebug("BoredomBreaker::pbSettingsClicked()");
    HWND m_hWnd = (HWND)(this->winId());
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

void BoredomBreaker::doActivateL2()
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

void BoredomBreaker::keyGlobalPressed(DWORD vkCode)
{

    qDebug("vkCode BoredomBreaker::keyGlobalPressed(DWORD vkCode=%d)", (int) vkCode);
    hk->keyPressed(vkCode);
}

void BoredomBreaker::keyGlobalReleased(DWORD vkCode)
{
    qDebug("vkCode BoredomBreaker::keyGlobalReleased(DWORD vkCode=%d)", (int) vkCode);


    int vkID = hk->keyReleased(vkCode);
    if(vkID < 0xFF){
         int b_control = vkID / K_NUM;
         int k_action = vkID - b_control * K_NUM;
         if(b_control == B_ONOFF){
             switch(k_action){
                 case K_DISABLE:
                     qDebug("vkCode K_DISABLE");
                     emit doSetState(false);
                     break;
                 case K_ENABLE:
                     qDebug("vkCode K_ENABLE");
                     emit doSetState(true);
                     break;
                 case K_TOGGLE:
                     qDebug("vkCode K_TOGGLE");
                     emit doSetState(!ui->cbDongle->isChecked());
                     break;
             }
         } else {
             switch(k_action){
                 case K_DISABLE:
                     qDebug("vkCode K_DISABLE");
                     enableGroup(b_control-1,false);
                     break;
                 case K_ENABLE:
                     qDebug("vkCode K_ENABLE");
                     enableGroup(b_control-1,true);
                     break;
                 case K_TOGGLE:
                     qDebug("vkCode K_TOGGLE");
                     enableGroup(b_control-1,!keyenable2[b_control-1]->isChecked());
                     break;
             }
         }
     }


}


void BoredomBreaker::cbDongleClicked(bool checked){
    qDebug("BoredomBreaker::cbDongleClicked(bool checked): %d", checked);
    emit doSetState(checked);

}

void BoredomBreaker::cbCtrlShiftClicked(bool checked){
    qDebug("BoredomBreaker::cbCtrlShiftClicked(bool checked: %d", checked);
    emit doSetModifier(ui->cbCtrl->isChecked(), ui->cbShift->isChecked());
}

void BoredomBreaker::cmbCondSetListTextChanged(const QString &text){
    qDebug("BoredomBreaker::cmbCondSetListTextChanged: %s", text.toStdString().c_str());
    int l2_index = ui->cmbWinList->currentIndex();
    if(!isValidIndex(l2_index))return;
    int index = ui->cmbCondSetList->currentIndex();
    if(!l2list[l2_index]->isValidIndex(index)) return;

    l2list[l2_index]->getCurrentSettings()->nic = text;
    ui->cmbCondSetList->setItemText(index, l2list[l2_index]->getNic());
}

//cbKeyEnableClicked
void BoredomBreaker::cbKeyEnableClicked(bool checked){
    qDebug("BoredomBreaker::cbKeyEnableClicked(bool checked): %d", checked);
    int index = ui->cmbWinList->currentIndex();
    if(!isValidIndex(index))return;
    QCheckBox* cb = dynamic_cast<QCheckBox*>(QObject::sender());
    if( cb != NULL )
    {
        int i = 0;
        while( (i< 48) && keyenable[i] != cb){i++;}
        if(i<48){
            l2list[index]->getCurrentSettings()->condition[i]->setState(checked);
            emit doSendKeyToDongle(i);
        }
    }
}

void BoredomBreaker::cbKeyEnableBxClicked(bool checked){
    qDebug("BoredomBreaker::cbKeyEnableBxClicked(bool checked): %d", checked);
    QCheckBox* cb = dynamic_cast<QCheckBox*>(QObject::sender());
    if( cb != NULL )
    {
        int i = 0;
        while( (i < GROUPSNUM) && keyenable2[i] != cb){i++;}
        if(i<GROUPSNUM){
            enableGroup(i, checked);
        }
    }
}



void BoredomBreaker::enableGroup(int group, bool state){
    qDebug("BoredomBreaker::enableGroup(int group): %d", group);
    keyenable2[group]->setChecked (state);
    emit setDongleGroupState(group, state);
}



bool BoredomBreaker::isValidIndex(int index){
    if((index == -1)||(l2list.isEmpty())||(index >= l2list.size()))return false;
    return true;
}

void BoredomBreaker::pbKeySettingsClicked(){
    qDebug("BoredomBreaker::pbKeySettingsClicked()");
    QPushButton* pb = dynamic_cast<QPushButton*>(QObject::sender());
    if( pb != NULL )
    {
        int i = 0;
        while( (i< 48) && keysettings[i] != pb){i++;}

        if(i<48){
            int index = ui->cmbWinList->currentIndex();
            if(!isValidIndex(index))return;

            KeyCondition cond(".");
            cond = *(l2list[index]->getCurrentSettings()->condition[i]);

            KeySettingsDialog dlg(&cond, l2list[index], i);
            if(dlg.exec() == QDialog::Accepted){
               *(l2list[index]->getCurrentSettings()->condition[i]) = cond;
            }
            emit doSendKeyToDongle(i);
        }
    }
}

void BoredomBreaker::pbAddClicked(){
    qDebug("BoredomBreaker::pbAddClicked");
    int index = ui->cmbWinList->currentIndex();
    if(!isValidIndex(index))return;

    QString file_name = QFileDialog::getOpenFileName(this, QString::fromUtf8("Имя файла"), "*.cfg");
    if(file_name.isEmpty() || file_name.isNull()) return;
    l2list[index]->AddConfig(file_name);
    pbToDongleClicked();
    ui->cmbCondSetList->addItem(l2list[index]->getCurrentSettings()->nic);
    ui->cmbCondSetList->setCurrentIndex(l2list[index]->activeCondSet);
    cmbCondSetListActivated(l2list[index]->activeCondSet);
}

void BoredomBreaker::pbLoadProjectClicked(){
    qDebug("BoredomBreaker::pbLoadProjectClicked");
    clicker->show();
    int index = ui->cmbWinList->currentIndex();
    if(!isValidIndex(index))return;

    QString file_name = QFileDialog::getOpenFileName(this, QString::fromUtf8("Имя файла"), "*.bbproj");
    if(file_name.isEmpty() || file_name.isNull()) return;
    l2list[index]->LoadProject(file_name);
    cmbWinListActivated(index);
    ui->cmbCondSetList->setCurrentIndex(l2list[index]->activeCondSet);
    cmbCondSetListActivated(l2list[index]->activeCondSet);
    pbToDongleClicked();
}

void BoredomBreaker::pbSaveProjectClicked(){
    qDebug("BoredomBreaker::pbSaveProjectClicked");

    int index = ui->cmbWinList->currentIndex();
    if(!isValidIndex(index))return;
    QString file_name = QFileDialog::getSaveFileName(this, QString::fromUtf8("Имя файла"), l2list[index]->project_file_name);
    if(file_name.isEmpty() || file_name.isNull()) return;
    qDebug("filename: %s", file_name.toStdString().c_str());
    l2list[index]->SaveProject(file_name);
    return;
}


void BoredomBreaker::pbLoadClicked(){
    qDebug("BoredomBreaker::pbLoadClicked");
    int index = ui->cmbWinList->currentIndex();
    if(!isValidIndex(index))return;

    QString file_name = QFileDialog::getOpenFileName(this, QString::fromUtf8("Имя файла"), "*.cfg");
    if(file_name.isEmpty() || file_name.isNull()) return;
    l2list[index]->LoadConfig(file_name);
    pbToDongleClicked();
    ui->cmbCondSetList->setCurrentIndex(l2list[index]->activeCondSet);
    cmbCondSetListActivated(l2list[index]->activeCondSet);
}

void BoredomBreaker::pbSaveClicked(){
    qDebug("BoredomBreaker::pbSaveClicked");

    int index = ui->cmbWinList->currentIndex();
    if(!isValidIndex(index))return;
    QString file_name = QFileDialog::getSaveFileName(this, QString::fromUtf8("Имя файла"), l2list[index]->getCurrentSettings()->settings_file_name);
    if(file_name.isEmpty() || file_name.isNull()) return;
    qDebug("filename: %s", file_name.toStdString().c_str());
    l2list[index]->SaveConfig(file_name);
    return;
}

void BoredomBreaker::pbToDongleClicked(){
    qDebug("BoredomBreaker::pbToDongleClicked");
    int index = ui->cmbWinList->currentIndex();
    if(!isValidIndex(index))return;
    emit doSaveAllToDongle();
}

void BoredomBreaker::pbJumpToBootloaderClicked(){
    qDebug("BoredomBreaker::pbJumpToBootloaderClicked");
    emit doJumpToBootloader();
}

void BoredomBreaker::pbEnumerateClicked(){
    qDebug("BoredomBreaker::pbEnumerateClicked()");
    enumerateL2();
}

void BoredomBreaker::pbFindBarsClicked(){
    qDebug("BoredomBreaker::pbFindBarsClicked()");
    int index = ui->cmbWinList->currentIndex();
    if(!isValidIndex(index))return;
    l2list[index]->resetBars();
    l2list[index]->resetSkillbar();
}

void BoredomBreaker::cmbCondSetListActivated(int index){

    qDebug("BoredomBreaker::cmbWinListActivated");
    int l2_index = ui->cmbWinList->currentIndex();
    if(!isValidIndex(l2_index))return;

    if(!l2list[l2_index]->isValidIndex(index)) return;
    l2list[l2_index]->activateSettings(index);

    for(int i=0;i<48;i++)
    {
        keylabel[i]->setText(l2list[l2_index]->getConditionLabel(i));
        keyenable[i]->setToolTip(l2list[l2_index]->getConditionLabel(i));
        keylabel[i]->setToolTip(l2list[l2_index]->getConditionLabel(i));
        keysettings[i]->setToolTip(l2list[l2_index]->getConditionLabel(i));
        keyenable[i]->setChecked (l2list[l2_index]->getConditionState(i));

    }
    ui->lbConfFileName->setText(l2list[l2_index]->getCurrentSettings()->settings_file_name);
    ui->leNic->setText(l2list[l2_index]->getNic());

}

void BoredomBreaker::cmbWinListActivated(int index){

    qDebug("BoredomBreaker::cmbWinListActivated");
    if(!isValidIndex(index))return;

    ui->cmbWinList->setItemIcon(index, *l2list[index]->getIcon());
    ui->cmbWinList->setItemText(index, l2list[index]->getTitle());
    while(ui->cmbCondSetList->count() > 0) {
        ui->cmbCondSetList->removeItem(0);
    }

    if(!l2list[index]->cond_set_list.isEmpty()){
      for(int i=0; i < l2list[index]->cond_set_list.size(); i++) {
          ui->cmbCondSetList->addItem(l2list[index]->cond_set_list[i]->nic);
      }
    }

    cmbCondSetListActivated(l2list[index]->activeCondSet);
    emit setActiveL2W(l2list[index]);

    l2_parser->setActiveL2W(l2list[index]);
}

void BoredomBreaker::showDongleStatus(unsigned char d_stt, unsigned char g_stt, int updatetime)
{
    //qDebug("BoredomBreaker::showDongleStatus");

    //clicker->showDongleStatus(d_stt,  g_stt, updatetime);

    Q_UNUSED(updatetime);
    for(int i=0;i<GROUPSNUM;i++)
    {
        keyenable2[i]->setChecked((g_stt & (1<<(i))) > 0);
        keyenable2[i]->setEnabled (true);
    }

    ui->cbCtrl->setChecked((d_stt & (1 << DEVICE_CTRL)) > 0);
    ui->cbShift->setChecked((d_stt & (1 << DEVICE_SHIFT)) > 0);

    bool state = (d_stt & (1<<DEVICE_STATUS)) > 0;
    switch(state){
    case STATE_OFF:
        if( ui->cbDongle->isChecked() == true){
            //if(bEnableSound) QSound::play("sounds/off.wav");
           //  QSound::play("sounds/on.wav");
        }
        ui->cbDongle->setChecked(false);
        ui->cbDongle->setEnabled(true);
        break;
    case STATE_ON:
        if( ui->cbDongle->isChecked() == false){
            //QSound::play("sounds/off.wav");
             //if(bEnableSound) QSound::play("sounds/on.wav");
        }
        ui->cbDongle->setChecked(true);
        ui->cbDongle->setEnabled(true);
        break;
     default:
        ui->cbDongle->setChecked(false);
        ui->cbDongle->setEnabled(false);
        break;
    }



    int index = ui->cmbWinList->currentIndex();
    if(!isValidIndex(index))return;
    QString label = "";

    switch(l2list[index]->getTargetType()){
        case  TARGETMEORPET:
            label = "ME OR PET";
            break;
        case  TARGETCHAR:
            label = "CHAR OR NPC";
            break;
        case  TARGETMOB:
            label = "MOB";
            break;
        case  NOTARGET:
            label = "NO TARGET";
            break;
        default:
            label = "UNKNOWN";
            break;
    }

    ui->lbTargetType->setText(label);

    for(int j = idCP; j < BARNUM; j++ ){
        int xp = l2list[index]->getXP(j);
        if(xp >=0 && xp <= 100){
            pb[j]->setValue(xp);
            pb[j]->setStyleSheet(StyleSheet[j]);
        } else {
            pb[j]->setValue(100);
            pb[j]->setStyleSheet(StyleSheet[BARNUM]);
        }
    }

    QColor color = *l2list[index]->getTokenColor();
    QPalette sample_palette;
    sample_palette.setColor(QPalette::Window, color.rgb());
    if(l2list[index]->getTokenState() != TOKEN_NONE) {
        sample_palette.setColor(QPalette::WindowText, Qt::green);
    } else {
        sample_palette.setColor(QPalette::WindowText, color.rgb());
    }

    ui->lbStar->setAutoFillBackground(true);
    ui->lbStar->setPalette(sample_palette);
    ui->cmbWinList->setItemIcon(index, *l2list[index]->getIcon());
    ui->cmbWinList->setItemText(index, l2list[index]->getTitle());
}


void BoredomBreaker::showParserStatus(int updatetime,  L2Window* l2w){
    qDebug("BoredomBreaker::showParserStatus(int updatetime) %d", updatetime);
    Q_UNUSED(l2w);

    ellipsed_time=((ellipsed_time*5)+updatetime)/6;
    QString label;
    QTextStream(&label) << ellipsed_time << " ms";
    ui->lb_ellipsed_time->setText(label);

    int index = ui->cmbWinList->currentIndex();
    if(!isValidIndex(index))return;

    for(int i = 0 ; i < KEYNUM; i++){
        if(l2list[index]->getConditionState(i)){
            if(l2list[index]->isSkillConditionRdy(i)) {
                keyenable[i]->setStyleSheet(StyleSheetCheckBox[3]); // GREEN
            } else {
                keyenable[i]->setStyleSheet(StyleSheetCheckBox[1]); // RED
            }
        } else {

           keyenable[i]->setStyleSheet(StyleSheetCheckBox[4]); // GRAY
        }
    }
}


void BoredomBreaker::enumerateL2(){
    qDebug("BoredomBreaker::enumerateL2()");
    if((BOOL)EnumWindows(&EnumWindowsProc,reinterpret_cast<long int>(this)))
    {
            qDebug("EnumWindows - ok");
    }else{qDebug("EnumWindows - not ok");}

}

void BoredomBreaker::addL2Window(HWND hwnd){
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
