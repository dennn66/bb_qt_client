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
    qDebug("BoredomBreaker::BoredomBreaker(QWidget *parent) Thread: %d", (int) QThread::currentThreadId());

    int id = qRegisterMetaType<QVector <HWND>>();

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

    for(int i = 0; i< KEYNUM; i++){
        connect(keyenable[i], SIGNAL(clicked(bool)), SLOT(cbKeyEnableClicked(bool)));
        connect(keysettings[i], SIGNAL(clicked()), SLOT(pbKeySettingsClicked()));
    }
    for(int i = 0; i< GROUPSNUM; i++){
        connect(keyenable2[i], SIGNAL(clicked(bool)), SLOT(cbKeyEnableBxClicked(bool)));
    }

    //l2_collection_thread = new QThread;
    //l2collection->moveToThread(l2_collection_thread);

    QThread* l2collection_thread = new QThread;
    L2Collection_Worker* l2collection_worker = new L2Collection_Worker();
    l2collection_worker->moveToThread(l2collection_thread);
    //connect(l2collection_worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    connect(l2collection_thread, SIGNAL(started()     ), l2collection_worker, SLOT(process()));
    connect(l2collection_worker, SIGNAL(finished()    ), l2collection_thread, SLOT(quit()));
    connect(l2collection_worker, SIGNAL(finished()    ), l2collection_worker, SLOT(deleteLater()));
    connect(l2collection_thread, SIGNAL(finished()    ), l2collection_thread, SLOT(deleteLater()));

    //connect(l2collection, SIGNAL(isL2Active(bool, int, int)                           ), SLOT(isL2Active(bool, int, int)                         ), Qt::DirectConnection);



    //while(worker->l2collection == NULL){;}
    l2collection = l2collection_worker->l2collection;

    //fix DongleWorker dongle_worker = new DongleWorker();

    //fix connect(dongle_worker, SIGNAL(showDongleStatusSig(unsigned char, unsigned char,int)), this,    SLOT(showDongleStatus(unsigned char, unsigned char, int)));

    //fix connect(this, SIGNAL(doSendKeyToDongle(int)), dongle_worker, SLOT(doSendKeyToDongle(int)));
    //fix connect(this, SIGNAL(doSetState(bool)), dongle_worker, SLOT(doSetState(bool)));
    //fix connect(this, SIGNAL(doSetModifier(bool, bool)), dongle_worker, SLOT(doSetModifier(bool, bool)));
    //fix connect(this, SIGNAL(doSaveAllToDongle()), dongle_worker, SLOT(doSaveAllToDongle()));
    //fix connect(this, SIGNAL(doJumpToBootloader()), dongle_worker, SLOT(doJumpToBootloader()));

    //fix connect(this, SIGNAL(setActiveL2W(int)), dongle_worker, SLOT(setActiveL2W(int)));
    //fix connect(this, SIGNAL(setDongleGroupState(int, bool)), dongle_worker, SLOT(doSetGroupState(int, bool)));


    clicker = new Clicker(l2collection, this);


    hk = new HotKeys(this);

    if(bEnableHotKey){
        kb = SystemKeyboardReadWrite::instance();
        kb->setConnected(true);
        connect(kb, SIGNAL(keyPressed(DWORD )), SLOT(keyGlobalPressed(DWORD)));
        connect(kb, SIGNAL(keyReleased(DWORD)), SLOT(keyGlobalReleased(DWORD)));
    }



    connect(l2collection, SIGNAL(resetWinView()         ),  SLOT(resetL2WindowsCombo()                       ), Qt::DirectConnection);
    connect(l2collection, SIGNAL(resetSetView()         ),  SLOT(resetSettingsCombo()                        ), Qt::DirectConnection);
    connect(l2collection, SIGNAL(resetKeyView()         ),  SLOT(resetKeys()                                 ), Qt::DirectConnection);

    connect(clicker, SIGNAL(popupSettings()             ),  SLOT(popupWindow()                               ), Qt::DirectConnection);

    connect(this, SIGNAL(resetL2Windows(QVector <HWND>*)), l2collection, SLOT(resetL2Windows(QVector <HWND>*)), Qt::DirectConnection);
    connect(this, SIGNAL(setActiveL2W(int)              ), l2collection, SLOT(setActiveL2W(int)              ), Qt::DirectConnection);
    connect(this, SIGNAL(setActiveSettings(int)         ), l2collection, SLOT(setActiveSettings(int)         ), Qt::DirectConnection);

    connect(this, SIGNAL(setDongleState(bool)           ), l2collection, SLOT(setDongleState(bool)           ), Qt::DirectConnection);
    connect(this, SIGNAL(setModifiers(bool, bool)       ), l2collection, SLOT(setModifiers(bool, bool)       ), Qt::DirectConnection);
    connect(this, SIGNAL(setNic(QString)                ), l2collection, SLOT(setNic(QString)                ), Qt::DirectConnection);
    connect(this, SIGNAL(setConditionState(int, bool)   ), l2collection, SLOT(setConditionState(int, bool)   ), Qt::DirectConnection);
    connect(this, SIGNAL(setGroupState(int, bool)       ), l2collection, SLOT(setGroupState(int, bool)       ), Qt::DirectConnection);
    connect(this, SIGNAL(editCondition(int)             ), l2collection, SLOT(editCondition(int)             ), Qt::DirectConnection);
    connect(this, SIGNAL(addConfig(QString)             ), l2collection, SLOT(addConfig(QString)             ), Qt::DirectConnection);
    connect(this, SIGNAL(loadProject(QString)           ), l2collection, SLOT(loadProject(QString)           ), Qt::DirectConnection);
    connect(this, SIGNAL(saveProject(QString)           ), l2collection, SLOT(saveProject(QString)           ), Qt::DirectConnection);
    connect(this, SIGNAL(loadConfig(QString)            ), l2collection, SLOT(loadConfig(QString)            ), Qt::DirectConnection);
    connect(this, SIGNAL(saveConfig(QString)            ), l2collection, SLOT(saveConfig(QString)            ), Qt::DirectConnection);
    connect(this, SIGNAL(saveAllToDongle()              ), l2collection, SLOT(saveAllToDongle()              ), Qt::DirectConnection);
    connect(this, SIGNAL(rebootDongle()                 ), l2collection, SLOT(rebootDongle()                 ), Qt::DirectConnection);
    connect(this, SIGNAL(findBars()                     ), l2collection, SLOT(findBars()                     ), Qt::DirectConnection);


    connect(l2collection, SIGNAL(showParserStatus(int)                                ), SLOT(showParserStatus(int)                              ), Qt::DirectConnection);
    connect(l2collection, SIGNAL(showDongleStatus(unsigned char, unsigned char,int)   ), SLOT(showDongleStatus(unsigned char, unsigned char, int)), Qt::DirectConnection);

    qDebug("BoredomBreaker start play");

    if(bEnableSound) QSound::play("sounds/on.wav");

    qDebug("BoredomBreaker stop play");

    enumerateL2();

    l2collection_thread->start();


}


BoredomBreaker::~BoredomBreaker()
{
    qDebug("BoredomBreaker::~BoredomBreaker");
    delete ui;
    delete clicker;
}

void BoredomBreaker::popupWindow()
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
                     //fix emit doSetState(false);
                     break;
                 case K_ENABLE:
                     qDebug("vkCode K_ENABLE");
                     //fix emit doSetState(true);
                     break;
                 case K_TOGGLE:
                     qDebug("vkCode K_TOGGLE");
                     //fix emit doSetState(!ui->cbDongle->isChecked());
                     break;
             }
         } else {
             switch(k_action){
                 case K_DISABLE:
                     qDebug("vkCode K_DISABLE");
                     emit setGroupState(b_control-1,false);
                     break;
                 case K_ENABLE:
                     qDebug("vkCode K_ENABLE");
                     emit setGroupState(b_control-1,true);
                     break;
                 case K_TOGGLE:
                     qDebug("vkCode K_TOGGLE");
                     emit setGroupState(b_control-1,!keyenable2[b_control-1]->isChecked());
                     break;
             }
         }
     }


}


void BoredomBreaker::cbDongleClicked(bool checked){
    qDebug("BoredomBreaker::cbDongleClicked(bool checked): %d", checked);
    emit setDongleState(checked);
}

void BoredomBreaker::cbCtrlShiftClicked(bool checked){
    qDebug("BoredomBreaker::cbCtrlShiftClicked(bool checked: %d", checked);
    emit setModifiers(ui->cbCtrl->isChecked(), ui->cbShift->isChecked());
}

void BoredomBreaker::cmbCondSetListTextChanged(const QString &text){
    qDebug("BoredomBreaker::cmbCondSetListTextChanged: %s", text.toStdString().c_str());
    //int l2_index = ui->cmbWinList->currentIndex();

    //if(!l2collection->isValidIndex(l2_index))return;
    int index = ui->cmbCondSetList->currentIndex();
    //if(!l2collection->isValidConditionIndex(index)) return;
    ui->cmbCondSetList->setItemText(index, text);
    emit setNic(text);
}

//cbKeyEnableClicked
void BoredomBreaker::cbKeyEnableClicked(bool checked){
    qDebug("BoredomBreaker::cbKeyEnableClicked(bool checked): %d", checked);
    //int index = ui->cmbWinList->currentIndex();
    //if(!l2collection->isValidIndex(index))return;
    QCheckBox* cb = dynamic_cast<QCheckBox*>(QObject::sender());
    if( cb != NULL )
    {
        int i = 0;
        while( (i< KEYNUM) && keyenable[i] != cb){i++;}
        if(i<KEYNUM){
            emit setConditionState(i, checked);
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
            emit setGroupState(i, checked);
        }
    }
}

void BoredomBreaker::pbKeySettingsClicked(){
    qDebug("BoredomBreaker::pbKeySettingsClicked()");
    QPushButton* pb = dynamic_cast<QPushButton*>(QObject::sender());
//    int l2_index = ui->cmbWinList->currentIndex();

//    if(!l2collection->isValidIndex(l2_index))return;
    if( pb != NULL )
    {
        int i = 0;
        while( (i< KEYNUM) && keysettings[i] != pb){i++;}
        if(i<KEYNUM) emit editCondition(i);
    }
}

/*
void BoredomBreaker::editCondition(int condnum){
    if(!isValidIndex(activeIndex)) return;
    KeyCondition cond(".");
    cond = *(l2list[activeIndex]->conditionmanager->getCurrentSettings()->condition[condnum]);

    KeySettingsDialog dlg(&cond, l2list[activeIndex], condnum);
    if(dlg.exec() == QDialog::Accepted){
      *(l2list[activeIndex]->conditionmanager->getCurrentSettings()->condition[condnum]) = cond;
    }
}
*/

void BoredomBreaker::pbAddClicked(){
    qDebug("BoredomBreaker::pbAddClicked");
 //   int index = ui->cmbWinList->currentIndex();
//    if(!l2collection->isValidIndex(index))return;

    QString file_name = QFileDialog::getOpenFileName(this, QString::fromUtf8("Имя файла"), "*.cfg");
    if(file_name.isEmpty() || file_name.isNull()) return;
    emit addConfig(file_name);

    //int activeCondSet = l2collection->addConfig(file_name);
    //ui->cmbCondSetList->addItem(l2collection->getNic());
    //ui->cmbCondSetList->setCurrentIndex(activeCondSet);
    //cmbCondSetListActivated(activeCondSet);
}

void BoredomBreaker::pbLoadProjectClicked(){
    qDebug("BoredomBreaker::pbLoadProjectClicked");
    qDebug("BoredomBreaker::pbLoadProjectClicked Thread: %d", (int) QThread::currentThreadId());
    //fix clicker->show();
    //int index = ui->cmbWinList->currentIndex();
    //if(!l2collection->isValidIndex(index))return;

    QString   file_name = QFileDialog::getOpenFileName(this, QString::fromUtf8("Имя файла"), "*.bbproj");
    if(file_name.isEmpty() || file_name.isNull()) return;
    emit loadProject(file_name);
    //int activeCondSet = l2collection->loadProject(file_name);

    //cmbWinListActivated(index);

    //ui->cmbCondSetList->setCurrentIndex(activeCondSet);
    //cmbCondSetListActivated(activeCondSet);
}

void BoredomBreaker::pbSaveProjectClicked(){
    qDebug("BoredomBreaker::pbSaveProjectClicked");
    int index = ui->cmbWinList->currentIndex();
    if(!l2collection->isValidIndex(index))return;
    QString file_name = QFileDialog::getSaveFileName(this, QString::fromUtf8("Имя файла"), l2collection->getProject_file_name());
    if(file_name.isEmpty() || file_name.isNull()) return;
    qDebug("filename: %s", file_name.toStdString().c_str());
    emit saveProject(file_name);
}


void BoredomBreaker::pbLoadClicked(){
    qDebug("BoredomBreaker::pbLoadClicked");
    qDebug("BoredomBreaker::pbLoadClicked Thread: %d", (int) QThread::currentThreadId());

    //int index = ui->cmbWinList->currentIndex();
    //if(!l2collection->isValidIndex(index))return;

    QString file_name = QFileDialog::getOpenFileName(this, QString::fromUtf8("Имя файла"), "*.cfg");
    if(file_name.isEmpty() || file_name.isNull()) return;
    emit loadConfig(file_name);
    //int activeCondSet = l2collection->loadConfig(file_name);
    //ui->cmbCondSetList->setCurrentIndex(activeCondSet);
    //cmbCondSetListActivated(activeCondSet);
}

void BoredomBreaker::pbSaveClicked(){
    qDebug("BoredomBreaker::pbSaveClicked");

    int index = ui->cmbWinList->currentIndex();
    if(!l2collection->isValidIndex(index))return;
    QString file_name = QFileDialog::getSaveFileName(this, QString::fromUtf8("Имя файла"), l2collection->getCurrentSettings_file_name());
    if(file_name.isEmpty() || file_name.isNull()) return;

    qDebug("filename: %s", file_name.toStdString().c_str());
    emit saveConfig(file_name);
    //l2collection->saveConfig(file_name);
}

void BoredomBreaker::pbToDongleClicked(){
    qDebug("BoredomBreaker::pbToDongleClicked");
    emit saveAllToDongle();
    //l2collection->saveAllToDongle();
}

void BoredomBreaker::pbJumpToBootloaderClicked(){
    qDebug("BoredomBreaker::pbJumpToBootloaderClicked");
    emit rebootDongle();
}

void BoredomBreaker::pbEnumerateClicked(){
    qDebug("BoredomBreaker::pbEnumerateClicked()");
    enumerateL2();
}

void BoredomBreaker::pbFindBarsClicked(){
    qDebug("BoredomBreaker::pbFindBarsClicked()");
    //int index = ui->cmbWinList->currentIndex();
    //if(!l2collection->isValidIndex(index))return;
    emit findBars();
    //l2collection->findBars();
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



    if(!l2collection->isValidIndex(index))return;
    QString label = "";

    switch(l2collection->getTargetType()){
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
        int xp = l2collection->getXP(j);
        if(xp >=0 && xp <= 100){
            pb[j]->setValue(xp);
            pb[j]->setStyleSheet(StyleSheet[j]);
        } else {
            pb[j]->setValue(100);
            pb[j]->setStyleSheet(StyleSheet[BARNUM]);
        }
    }

    QColor color = Qt::white; //fix *l2collection->getTokenColor();
    QPalette sample_palette;
    sample_palette.setColor(QPalette::Window, color.rgb());
    if(l2collection->getTokenState() != TOKEN_NONE) {
        sample_palette.setColor(QPalette::WindowText, Qt::green);
    } else {
        sample_palette.setColor(QPalette::WindowText, color.rgb());
    }

    ui->lbStar->setAutoFillBackground(true);
    ui->lbStar->setPalette(sample_palette);
    //fix ui->cmbWinList->setItemIcon(index, *l2collection->getIcon());
    ui->cmbWinList->setItemText(index, l2collection->getCurrentTitle());
}


void BoredomBreaker::showParserStatus(int updatetime){
    qDebug("BoredomBreaker::showParserStatus(int updatetime) %d", updatetime);
    qDebug("Thread: %d", (int) QThread::currentThreadId());


    ellipsed_time=((ellipsed_time*5)+updatetime)/6;
    QString label;
    QTextStream(&label) << ellipsed_time << " ms";
    ui->lb_ellipsed_time->setText(label);

    int index = ui->cmbWinList->currentIndex();



    if(!l2collection->isValidIndex(index))return;

    for(int i = 0 ; i < KEYNUM; i++){
/*
        if(l2collection->isActionTurnedOn(i)){
            if(l2collection->isActionConditionRdy(i)) {
                keyenable[i]->setStyleSheet(StyleSheetCheckBox[3]); // GREEN
            } else {
                keyenable[i]->setStyleSheet(StyleSheetCheckBox[1]); // RED
            }
        } else {

           keyenable[i]->setStyleSheet(StyleSheetCheckBox[4]); // GRAY
        }
*/
    }
}

/*
void BoredomBreaker::resetL2Windows(){
    qDebug("BoredomBreaker::resetL2Windows()");

    l2collection->resetL2Windows(hwnd_list);
    resetL2WindowsCombo();
}
*/


void BoredomBreaker::cmbCondSetListActivated(int index){

    qDebug("BoredomBreaker::cmbWinListActivated");
    emit setActiveSettings(index);
}

void BoredomBreaker::cmbWinListActivated(int index){

    qDebug("BoredomBreaker::cmbWinListActivated");
    emit setActiveL2W(index);

}

void BoredomBreaker::resetL2WindowsCombo(){
    qDebug("BoredomBreaker::resetL2WindowsCombo(): ");
    ui->cmbWinList->clear();
    //example while(ui->cmbCondSetList->count() > 0) { ui->cmbCondSetList->removeItem(0);}
    for(int l2winnum = 0; l2winnum < l2collection->get_l2list_size(); l2winnum++){

        ui->cmbWinList->addItem(l2collection->getTitle(l2winnum));
    }
    if(ui->cmbWinList->count() > 0){
        ui->cmbWinList->setCurrentIndex(l2collection->getActiveL2W());
    }
    ui->lbConfFileName->setText(l2collection->getCurrentSettings_file_name());
    resetSettingsCombo();
}

void BoredomBreaker::resetSettingsCombo(){
    qDebug("BoredomBreaker::resetSettingsCombo()");
    ui->cmbCondSetList->clear();
    for(int cond_index=0; cond_index < l2collection->getCond_list_size(); cond_index++) {
        ui->cmbCondSetList->addItem(l2collection->getNic(cond_index));
    }
    if(ui->cmbCondSetList->count() > 0){
        ui->cmbCondSetList->setCurrentIndex(l2collection->getActiveCondSet());
    }
    ui->leNic->setText(l2collection->getNic());
    resetKeys();
}

void BoredomBreaker::resetKeys(){
    qDebug("BoredomBreaker::resetSettingsKeys()");

    for(int i=0;i<KEYNUM;i++)
    {
        keylabel[i]->setText(l2collection->getConditionLabel(i));
        keyenable[i]->setToolTip(l2collection->getConditionLabel(i));
        keylabel[i]->setToolTip(l2collection->getConditionLabel(i));
        keysettings[i]->setToolTip(l2collection->getConditionLabel(i));
        keyenable[i]->setChecked (l2collection->isActionTurnedOn(i));
    }
}

void BoredomBreaker::startL2enumerating(){
    qDebug("BoredomBreaker::startL2enumerating()");
    hwnd_list.clear();

}

void BoredomBreaker::addL2HWND(HWND hwnd){
    qDebug("BoredomBreaker::addL2HWND(HWND hwnd): %d", (int) hwnd);
    hwnd_list.append(hwnd);
}


void BoredomBreaker::enumerateL2(){
    qDebug("BoredomBreaker::enumerateL2()");
    startL2enumerating();
    if((BOOL)EnumWindows(&EnumWindowsProc,reinterpret_cast<long int>(this)))
    {
        emit resetL2Windows(&hwnd_list);
        qDebug("EnumWindows - ok");
    }else{qDebug("EnumWindows - not ok");}

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
            bb->addL2HWND(hwnd);
        }
        return TRUE;
}
