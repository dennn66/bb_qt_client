//123
#include "boredombreaker.h"
#include "ui_boredombreaker.h"


const char* BoredomBreaker::StyleSheetCheckBox[5] = {
    "QCheckBox::indicator  {background: #805300 ;}",  //YELLOW
    "QCheckBox::indicator  {background: #881D18 ;}",  //RED
    "QCheckBox::indicator  {background: #03327E ;}",  //BLUE
    "QCheckBox::indicator  {background: #318A10 ;}",  //GREEN
    "QCheckBox::indicator  {background: #7F7F7F ;}"   //GREY
};

const char* BoredomBreaker::StyleSheetLabel[6] = {
    "QLabel  {background: #805300 ;}",  //YELLOW
    "QLabel  {background: #881D18 ;}",  //RED
    "QLabel  {background: #03327E ;}",  //BLUE
    "QLabel  {background: #318A10 ;}",  //GREEN
    "QLabel  {background: #7F7F7F ;}"   //GREY
    "QLabel  {background: #FFFFFF ;}"   //WHITE
};

#define TELEBOT_TOKEN "Set token in bb.ini -> MAIN/TelegramToken"


BoredomBreaker::BoredomBreaker(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BoredomBreaker)
{

    qRegisterMetaType<QVector <HWND>>();
    qRegisterMetaType<HWND>("HWND");
    qRegisterMetaType<QVector <QString>>();

    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    ellipsed_time = 0;


    //LOAD CONFIG BB.ini
    QSettings sett("bb.ini", QSettings::IniFormat);
    bEnableSound = sett.value("MAIN/EnableSound", 1).toBool();
    bEnableTelegram = sett.value("MAIN/EnableTelegram", 0).toBool();
    QString tbot_token = sett.value("MAIN/TelegramToken", TELEBOT_TOKEN).toString();

    project_file_name  = "default.bbproj";
    settings_file_name = "default.cfg";

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

    lstAllSkills  = new QListWidget(this);

    lstAllSkills->resize( 4 + 32*12 + 6*(12-1) + 8, 2 + 32*4 + 5*(3 - 1) + 18);
    lstAllSkills->move(5, 5);
    lstAllSkills->setIconSize( QSize( 32, 32 ) );
    lstAllSkills->setViewMode( QListWidget::IconMode );
    lstAllSkills->setSelectionMode(QAbstractItemView::SingleSelection);
    lstAllSkills->setDragEnabled(false);
    lstAllSkills->setDragDropMode(QAbstractItemView::NoDragDrop);
    lstAllSkills->viewport()->setAcceptDrops(false);
    lstAllSkills->setDropIndicatorShown(false);
    lstAllSkills->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    lstAllSkills->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QImage noimage(32,32,QImage::Format_ARGB32);
    noimage.fill(Qt::gray);
    for(int row = 0; row < 4; row++){
        for(int col=0; col < 12; col++){
            int i = (3-row)*12+col;
            if(i<KEYNUM){
                listNoUseSkill[i] = new QListWidgetItem;
                if(!noimage.isNull()) listNoUseSkill[i]->setIcon( QPixmap::fromImage(noimage));
                listNoUseSkill[i]->setFlags(  Qt::ItemIsEnabled);
                lstAllSkills->addItem(listNoUseSkill[i]);
            }
        }
    }


    dongle = new Dongle();
    QThread* dongle_thread = new QThread;
    dongle->moveToThread(dongle_thread);

    QThread* l2_parser_thread = new QThread;
    l2_parser = new L2parser();
    l2_parser->moveToThread(l2_parser_thread);

    clicker = new Clicker;


    QThread* hk_thread = new QThread;
    hk = new HotKeys();
    hk->moveToThread(hk_thread);

    QThread* tele_bot_thread = new QThread;
    tele_bot = new TelegramBot(tbot_token);
    tele_bot->moveToThread(tele_bot_thread);

    QThread* evprocessor_thread = new QThread;
    evprocessor = new EventProcessor(bEnableSound);
    evprocessor->moveToThread(evprocessor_thread);

    connect(l2_parser_thread,   SIGNAL(started())       , l2_parser,          SLOT(process()));
    connect(l2_parser,          SIGNAL(finished())      , l2_parser_thread,   SLOT(quit()));
    connect(l2_parser,          SIGNAL(finished())      , l2_parser,          SLOT(deleteLater()));
    connect(l2_parser_thread,   SIGNAL(finished())      , l2_parser_thread,   SLOT(deleteLater()));


    connect(dongle_thread,      SIGNAL(started())       , dongle,             SLOT(process()));
    connect(dongle,             SIGNAL(finished())      , dongle_thread,      SLOT(quit()));
    connect(dongle,             SIGNAL(finished())      , dongle,             SLOT(deleteLater()));
    connect(dongle_thread,      SIGNAL(finished())      , dongle_thread,      SLOT(deleteLater()));


    connect(hk_thread,          SIGNAL(started())       , hk,                 SLOT(process()));
    connect(hk,                 SIGNAL(finished())      , hk_thread,          SLOT(quit()));
    connect(hk,                 SIGNAL(finished())      , hk,                 SLOT(deleteLater()));
    connect(hk_thread,          SIGNAL(finished())      , hk_thread,          SLOT(deleteLater()));

    if(bEnableTelegram)
    {

        connect(tele_bot_thread,    SIGNAL(started())       , tele_bot,           SLOT(process()));
        connect(tele_bot,           SIGNAL(finished())      , tele_bot_thread,    SLOT(quit()));
        connect(tele_bot,           SIGNAL(finished())      , tele_bot,           SLOT(deleteLater()));
        connect(tele_bot_thread,    SIGNAL(finished())      , tele_bot_thread,    SLOT(deleteLater()));
    }
    connect(evprocessor_thread, SIGNAL(started())       , evprocessor,        SLOT(process()));
    connect(evprocessor,        SIGNAL(finished())      , evprocessor_thread, SLOT(quit()));
    connect(evprocessor,        SIGNAL(finished())      , evprocessor,        SLOT(deleteLater()));
    connect(evprocessor_thread, SIGNAL(finished())      , evprocessor_thread, SLOT(deleteLater()));



    connect(ui->pbLoadProject,  SIGNAL(clicked()),                          SLOT(pbLoadProjectClicked()));
    connect(ui->pbSaveProject,  SIGNAL(clicked()),                          SLOT(pbSaveProjectClicked()));
    connect(ui->pbLoad,         SIGNAL(clicked()),                          SLOT(pbLoadClicked()));
    connect(ui->pbSave,         SIGNAL(clicked()),                          SLOT(pbSaveClicked()));
    connect(ui->pbAdd,          SIGNAL(clicked()),                          SLOT(pbAddClicked()));
    connect(ui->pbEnumerate,    SIGNAL(clicked()),                          SLOT(startL2enumerating()));
    connect(ui->cmbWinList,     SIGNAL(activated(int)),                     SLOT(cmbWinListActivated(int)));
    connect(ui->cmbCondSetList, SIGNAL(activated(int)),                     SLOT(cmbCondSetListActivated(int)));

    for(int i = 0; i< GROUPSNUM; i++){
        connect(keyenable2[i],  SIGNAL(clicked(bool)),                      SLOT(cbKeyEnableBxClicked(bool)));
    }


    connect(lstAllSkills, SIGNAL(itemClicked(QListWidgetItem*)),                            SLOT(itemClicked(QListWidgetItem*)));
    connect(lstAllSkills, SIGNAL(itemDoubleClicked(QListWidgetItem*)),                      SLOT(itemDoubleClicked(QListWidgetItem*)));

    connect(this,         SIGNAL(toggleRuleState(int))                      , l2_parser,    SLOT(toggleRuleState(int))           , Qt::DirectConnection);
    connect(this,         SIGNAL(editRule(int))                             , l2_parser,    SLOT(editRule(int))                  , Qt::DirectConnection);
    connect(this,         SIGNAL(changeNic(const  QString&))                , l2_parser,    SLOT(changeNic(const  QString&))     , Qt::DirectConnection);

    connect(this,         SIGNAL(loadConfig (QString))                      , l2_parser,    SLOT(loadConfig (QString))           , Qt::DirectConnection);
    connect(this,         SIGNAL(saveProject(QString))                      , l2_parser,    SLOT(saveProject(QString))           , Qt::DirectConnection);
    connect(this,         SIGNAL(loadProject(QString))                      , l2_parser,    SLOT(loadProject(QString))           , Qt::DirectConnection);
    connect(this,         SIGNAL(saveConfig (QString))                      , l2_parser,    SLOT(saveConfig (QString))           , Qt::DirectConnection);
    connect(this,         SIGNAL(setActiveCondIndex(int))                   , l2_parser,    SLOT(setActiveCondIndex(int))        , Qt::DirectConnection);
    connect(this,         SIGNAL(setActiveL2Index  (int))                   , l2_parser,    SLOT(setActiveL2Index  (int))        , Qt::DirectConnection);

    connect(ui->pbDongle, SIGNAL(clicked())                                 , l2_parser,    SLOT(send_all_keys_to_dongle())      , Qt::DirectConnection);
    connect(ui->pbJumpToBootloader, SIGNAL(clicked())                       , dongle,       SLOT(jump_to_bootloader())           , Qt::DirectConnection);
    connect(ui->cbDongle, SIGNAL(clicked(bool))                             , dongle,       SLOT(set_operation_state(bool))      , Qt::DirectConnection);
    connect(ui->leNic,    SIGNAL(textChanged(const QString &))              , l2_parser,    SLOT(changeNic(const QString &))     , Qt::DirectConnection);
    connect(ui->pbFindBars, SIGNAL(clicked())                               , l2_parser,    SLOT(resetBars())                    , Qt::DirectConnection);
    connect(clicker,      SIGNAL(resetBars())                               , l2_parser,    SLOT(resetBars())                    , Qt::DirectConnection);
    connect(ui->cbCtrl,   SIGNAL(clicked(bool))                             , dongle,       SLOT(set_ctrl(bool))                 , Qt::DirectConnection);
    connect(ui->cbShift,  SIGNAL(clicked(bool))                             , dongle,       SLOT(set_shift(bool))                , Qt::DirectConnection);
    connect(clicker,      SIGNAL(set_ctrl(bool))                            , dongle,       SLOT(set_ctrl(bool))                 , Qt::DirectConnection);
    connect(clicker,      SIGNAL(set_shift(bool))                           , dongle,       SLOT(set_shift(bool))                , Qt::DirectConnection);
    connect(hk,           SIGNAL(set_shift(bool))                           , dongle,       SLOT(set_shift(bool))                , Qt::DirectConnection);
    connect(hk,           SIGNAL(toggle_shift())                            , dongle,       SLOT(toggle_shift())                 , Qt::DirectConnection);
    connect(evprocessor,  SIGNAL(set_dongle_mode(bool))                     , dongle,       SLOT(set_mode(bool))                 , Qt::DirectConnection);
    connect(evprocessor,  SIGNAL(set_mouse_report(qint8, qint8, bool, bool, bool)),
                                                                              dongle,       SLOT(set_mouse_report(qint8, qint8, bool, bool, bool)),                                                                                                                                   Qt::DirectConnection);

    connect(this,       SIGNAL(redraw())                                     , l2_parser,   SLOT(redraw())                                      , Qt::DirectConnection);

    connect(l2_parser,  SIGNAL(updateGroupState(int,bool))                   , this   ,     SLOT(updateGroupState(int,bool))                    );
    connect(l2_parser,  SIGNAL(updateGroupState(int,bool))                   , clicker,     SLOT(updateGroupState(int,bool))                    );
    connect(l2_parser,  SIGNAL(set_dongle_skill_state(int,bool))             , dongle,      SLOT(set_dongle_skill_state(int,bool))              , Qt::DirectConnection);

    connect(this,       SIGNAL(setGroupState(int,bool))                      , l2_parser,   SLOT(setGroupState(int,bool))                       , Qt::DirectConnection);
    connect(clicker,    SIGNAL(setGroupState(int,bool))                      , l2_parser,   SLOT(setGroupState(int,bool))                       , Qt::DirectConnection);
    connect(hk,         SIGNAL(setGroupState(int,bool))                      , l2_parser,   SLOT(setGroupState(int,bool))                     , Qt::DirectConnection);
    connect(hk,         SIGNAL(toggleGroupState(int))                        , l2_parser,   SLOT(toggleGroupState(int))                         , Qt::DirectConnection);
    connect(this,       SIGNAL(resetL2Windows(QVector <HWND>*))              , l2_parser,   SLOT(resetL2Windows(QVector <HWND>*))               , Qt::DirectConnection);


    connect(l2_parser,  SIGNAL(showParserStatus(int, L2Window*, QImage))     , this,        SLOT(showParserStatus(int, L2Window*, QImage))      );
    connect(l2_parser,  SIGNAL(showParserStatus(int, L2Window*, QImage))     , clicker,     SLOT(showParserStatus(int, L2Window*, QImage))      );
    connect(l2_parser,  SIGNAL(showParserStatus(int, L2Window*, QImage))     , evprocessor, SLOT(showParserStatus(int, L2Window*, QImage))      );
    connect(clicker,    SIGNAL(popupBbWindow())                              , this,        SLOT(popupBbWindow())                               );
    if(bEnableTelegram)
    {
        connect(l2_parser,  SIGNAL(showParserStatus(int, L2Window*, QImage))     , tele_bot,    SLOT(showParserStatus(int, L2Window*, QImage))      );
        connect(evprocessor,SIGNAL(sendTextMessage(QString))                     , tele_bot,    SLOT(sendTextMessage(QString))                      );
        connect(tele_bot,   SIGNAL(popupWindow(HWND))                          , this,        SLOT(popupWindow(HWND))                           );
        connect(tele_bot,   SIGNAL(resurrect())                                  , evprocessor, SLOT(resurrect())                                   );
        connect(tele_bot,   SIGNAL(set_operation_state(bool))                    , dongle,      SLOT(set_operation_state(bool))                           , Qt::DirectConnection);
    }
    connect(hk,         SIGNAL(toggle_operation_state())                     , dongle,      SLOT(toggle_operation_state())                            , Qt::DirectConnection);
    connect(hk,         SIGNAL(set_operation_state(bool))                    , dongle,      SLOT(set_operation_state(bool))                           , Qt::DirectConnection);
    connect(clicker,    SIGNAL(set_operation_state(bool))                    , dongle,      SLOT(set_operation_state(bool))                           , Qt::DirectConnection);
    connect(this,       SIGNAL(set_operation_state(bool))                    , dongle,      SLOT(set_operation_state(bool))                           , Qt::DirectConnection);
    connect(evprocessor,SIGNAL(set_operation_state(bool))                    , dongle,      SLOT(set_operation_state(bool))                           , Qt::DirectConnection);
    connect(this,       SIGNAL(jump_to_bootloader())                         , dongle,      SLOT(jump_to_bootloader())                                , Qt::DirectConnection);


    connect(dongle,     SIGNAL(showStatus(unsigned char, int))               , clicker,     SLOT(showDongleStatus(unsigned char, int))                );
    connect(dongle,     SIGNAL(showStatus(unsigned char, int))               , this,        SLOT(showDongleStatus(unsigned char, int))                );
    connect(dongle,     SIGNAL(showStatus(unsigned char, int))               , l2_parser,   SLOT(showDongleStatus(unsigned char, int))                , Qt::DirectConnection);
    connect(dongle,     SIGNAL(showStatus(unsigned char, int))               , evprocessor, SLOT(showDongleStatus(unsigned char, int))                , Qt::DirectConnection);


    connect(l2_parser,  SIGNAL(updateConditiosList(QVector <QString>, int, QString))  , this   ,     SLOT(updateConditiosList(QVector <QString>, int, QString))   , Qt::DirectConnection);
    connect(l2_parser,  SIGNAL(updateL2WindowsList(QVector <QString>, int, QString))  , this   ,     SLOT(updateL2WindowsList(QVector <QString>, int, QString))   , Qt::DirectConnection);

    connect(l2_parser,  SIGNAL(setup_key(int, bool, unsigned char, float, float, float,  bool, bool)),
            dongle,     SLOT  (setup_key(int, bool, unsigned char, float, float, float,  bool, bool)), Qt::DirectConnection);

    emit redraw();

    startL2enumerating();

    //qDebug("BoredomBreaker start play");

    if(bEnableSound)PlaySound((LPCWSTR)"sounds/on.wav", NULL, SND_FILENAME);

    //qDebug("BoredomBreaker stop play");

    dongle_thread->start();
    l2_parser_thread->start();
    hk_thread->start();
    if(bEnableTelegram) tele_bot_thread->start();
    evprocessor_thread->start();
    lstAllSkills->show();
}


BoredomBreaker::~BoredomBreaker()
{
    //qDebug("BoredomBreaker::~BoredomBreaker");
    delete ui;
    delete clicker;
}

void BoredomBreaker::updateConditiosList(QVector <QString> list, int current_index, QString settings){
    qDebug()<< "BoredomBreaker::updateConditiosList(QVector <QString> list, int current_index, QString settings)";
    updateListbox(list, ui->cmbCondSetList, current_index);
    settings_file_name = settings;
    ui->lbConfFileName->setText(settings_file_name);
    qDebug()<<"ui->leNic->setText(ui->cmbCondSetList->currentText()) " << ui->cmbCondSetList->currentText();
    ui->leNic->setText(ui->cmbCondSetList->currentText());
}

void BoredomBreaker::updateL2WindowsList(QVector <QString> list, int current_index, QString project){
    updateListbox(list, ui->cmbWinList    , current_index);
    project_file_name  = project;
}
void BoredomBreaker::updateListbox(QVector <QString>  list, QComboBox* combo, int _current_index){
    qDebug()<<"BoredomBreaker::updateListbox(QVector <QString>  list, QComboBox* combo, int _current_index)";
    int i=0;

    while((i < combo->count()) && (i < list.count())){
       qDebug()<<"combo->setItemText(i, list[i]) " << i << " " << list[i];
       combo->setItemText(i, list[i]);
       i++;
    }
    while( i < combo->count()){
       combo->removeItem(i);
       i++;
    }

    while(i < list.count()){
       qDebug()<<"combo->addItem(list[i]) " << i << " " << list[i];
       combo->addItem(list[i]);
       i++;
    }
    int current_index = _current_index;
    if(current_index < 0 && combo->count() > 0) current_index = 0;
    qDebug()<<"combo->setCurrentIndex(current_index)) " << current_index;
    combo->setCurrentIndex(current_index);
}


void BoredomBreaker::itemClicked(QListWidgetItem* item){
    int key_index = 0;
    while( (key_index < KEYNUM) && (listNoUseSkill[key_index] != item)){key_index++;}
    if(key_index < KEYNUM ) { emit toggleRuleState(key_index);    }
}

void BoredomBreaker::itemDoubleClicked(QListWidgetItem* item){
    int key_index = 0;
    while( (key_index < KEYNUM) && (listNoUseSkill[key_index] != item)){key_index++;}
    if(key_index<KEYNUM){
        emit editRule(key_index);
    }
}

void BoredomBreaker::popupBbWindow()
{
    popupWindow((HWND)this->winId());
}

void BoredomBreaker::popupWindow(HWND hwnd)
{
    //qDebug("BoredomBreaker::doActivateL2W()");
    HWND m_hWnd = hwnd;
    //m_hWnd = (HWND)this->winId();
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

void BoredomBreaker::cbKeyEnableBxClicked(bool checked){
    //qDebug("BoredomBreaker::cbKeyEnableBxClicked(bool checked): %d", checked);
    QCheckBox* cb = dynamic_cast<QCheckBox*>(QObject::sender());
    if( cb != NULL )
    {
        int i = 0;
        while( (i < GROUPSNUM) && keyenable2[i] != cb){i++;}
        if(i<GROUPSNUM){ emit setGroupState(i, checked);   }
    }
}


void BoredomBreaker::pbLoadProjectClicked(){
    //qDebug("BoredomBreaker::pbLoadProjectClicked");
    QString file_name = QFileDialog::getOpenFileName(this, QString::fromUtf8("Имя файла"), "*.bbproj");
    if(file_name.isEmpty() || file_name.isNull()) return;
    emit loadProject(file_name);
}

void BoredomBreaker::pbSaveProjectClicked(){
    //qDebug("BoredomBreaker::pbSaveProjectClicked");
    QString file_name = QFileDialog::getSaveFileName(this, QString::fromUtf8("Имя файла"), project_file_name);
    if(file_name.isEmpty() || file_name.isNull()) return;
    emit saveProject(file_name);
}


void BoredomBreaker::pbSaveClicked(){
    //qDebug("BoredomBreaker::pbSaveClicked");
    QString file_name = QFileDialog::getSaveFileName(this, QString::fromUtf8("Имя файла"), settings_file_name);
    if(file_name.isEmpty() || file_name.isNull()) return;
    emit saveConfig(file_name);
}

void BoredomBreaker::pbLoadClicked(){
    //qDebug("BoredomBreaker::pbLoadClicked");
    QString file_name = QFileDialog::getOpenFileName(this, QString::fromUtf8("Имя файла"), "*.cfg");
    if(file_name.isEmpty() || file_name.isNull()) return;
    emit loadConfig(file_name);
}

void BoredomBreaker::pbAddClicked(){
    //qDebug("BoredomBreaker::pbAddClicked");
    QString file_name = QFileDialog::getOpenFileName(this, QString::fromUtf8("Имя файла"), "*.cfg");
    if(file_name.isEmpty() || file_name.isNull()) return;
    emit addConfig(file_name);
}

void BoredomBreaker::cmbCondSetListActivated(int index){

    qDebug() << "BoredomBreaker::cmbCondSetListActivated "<< index;
    qDebug() << "ui->leNic->setText(ui->cmbCondSetList->currentText()) "<< ui->cmbCondSetList->currentText();

    //ui->leNic->setText(ui->cmbCondSetList->currentText());
    emit setActiveCondIndex(index);
}

void BoredomBreaker::cmbWinListActivated(int l2_index){
    //qDebug("BoredomBreaker::cmbWinListActivated");
    emit setActiveL2Index(l2_index);
}

void BoredomBreaker::showDongleStatus(unsigned char d_stt,  int updatetime)
{
    Q_UNUSED(updatetime);
    ui->cbCtrl  ->setChecked((d_stt & (1 << DEVICE_CTRL  )) > 0);
    ui->cbShift ->setChecked((d_stt & (1 << DEVICE_SHIFT )) > 0);
    ui->cbDongle->setChecked((d_stt & (1 << DEVICE_STATUS)) > 0);
    ui->cbDongle->setEnabled(true);
}

void BoredomBreaker::updateGroupState(int num,  bool state){
    if(num <GROUPSNUM) {
        keyenable2[num]->setChecked(state);
        keyenable2[num]->setEnabled (true);
    }
}

void BoredomBreaker::showParserStatus(int updatetime,  L2Window* l2w, QImage clicker_bk){
    //qDebug("BoredomBreaker::showParserStatus(int updatetime) %d", updatetime);
    Q_UNUSED(clicker_bk);

    ellipsed_time=((ellipsed_time*5)+updatetime)/6;
    QString label;
    QTextStream(&label) << ellipsed_time << " ms";
    ui->lb_ellipsed_time->setText(label);

    if(l2w == NULL)return;

    switch(l2w->getTargetType()){
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

    QColor color = *l2w->getTokenColor();
    QPalette sample_palette;
    sample_palette.setColor(QPalette::Window, color.rgb());
    if(l2w->getTokenState() != TOKEN_NONE) {
        sample_palette.setColor(QPalette::WindowText, Qt::green);
    } else {
        sample_palette.setColor(QPalette::WindowText, color.rgb());
    }

    ui->lbStar->setAutoFillBackground(true);
    ui->lbStar->setPalette(sample_palette);
    ui->cmbWinList->setItemIcon(ui->cmbWinList->currentIndex(), *l2w->getIcon());

    for(int i = 0; i < KEYNUM; i ++){
        if(!l2w->getSkillbar()->getSkillImg(i)->isNull()) listNoUseSkill[i]->setIcon( QPixmap::fromImage(*l2w->getSkillbar()->getSkillImg(i)) );
        if(l2w->getCurrentSettings()->condition[i]->getState()){
            listNoUseSkill[i]->setFlags(  Qt::ItemIsEnabled);
        } else {
            listNoUseSkill[i]->setFlags( Qt::NoItemFlags);
        }
    }
}



void BoredomBreaker::addL2HWND(HWND hwnd){
    //qDebug("BoredomBreaker::addL2HWND(HWND hwnd): %d", (int) hwnd);
    hwnd_list.append(hwnd);
}


void BoredomBreaker::startL2enumerating(){
    //qDebug("BoredomBreaker::enumerateL2()");
    hwnd_list.clear();
    if((BOOL)EnumWindows(&EnumWindowsProc,reinterpret_cast<long int>(this)))
    {
        emit resetL2Windows(&hwnd_list);
        //qDebug("EnumWindows - ok");
    }else{
        qWarning("EnumWindows - fail");
    }

}

BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lParam)
{
        //qDebug("BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lParam)");
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
