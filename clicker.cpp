#include "clicker.h"
#include "ui_clicker.h"

const char* Clicker::StyleSheetCheckBox[5] = {
    "QCheckBox::indicator  {background: #805300 ;}",  //YELLOW
    "QCheckBox { color : white; }; QCheckBox::indicator  {background: #881D18 ;}",  //RED
    "QCheckBox::indicator  {background: #03327E ;}",  //BLUE
    "QCheckBox { color : white; }; QCheckBox::indicator  {background: #318A10 ;}",  //GREEN
    "QCheckBox::indicator  {background: #7F7F7F ;}"   //GREY
};

Clicker::Clicker(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Clicker)
{
    ui->setupUi(this);
    setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowTransparentForInput| Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);

    QPalette pal = this->palette();
    pal.setColor(QPalette::Window, QColor("#0100FF00"));
    this->setPalette(pal);

    bFindBarsIsPressed = false;
    bSettingsIsPressed = false;
    green_frame = new QImage(QSize(51,23), QImage::Format_ARGB32);
    QPainter p;
    p.begin(green_frame);
    p.setPen(QPen(QColor("#4400FF00")));
    p.setBrush(QBrush(QColor("#4400FF00"), Qt::NoBrush));
    p.drawRect(QRect(0,0,50,22));
    p.setPen(QPen(QColor("#EE00FF00")));
    p.setBrush(QBrush(QColor("#8800FF00"), Qt::NoBrush));
    p.drawRect(QRect(1,1,48,20));
    p.setPen(QPen(QColor("#4400FF00")));
    p.setBrush(QBrush(QColor("#0100FF00"), Qt::SolidPattern));
    p.drawRect(QRect(2,2,46,18));
    p.end();
    red_frame = new QImage(QSize(51,23), QImage::Format_ARGB32);
    p.begin(red_frame);
    p.setPen(QPen(QColor("#44FF0000")));
    p.setBrush(QBrush(QColor("#44FF0000"), Qt::NoBrush));
    p.drawRect(QRect(0,0,50,22));
    p.setPen(QPen(QColor("#EEFF0000")));
    p.setBrush(QBrush(QColor("#88FF0000"), Qt::NoBrush));
    p.drawRect(QRect(1,1,48,20));
    p.setPen(QPen(QColor("#44FF0000")));
    p.setBrush(QBrush(QColor("#01FF0000"), Qt::SolidPattern));
    p.drawRect(QRect(2,2,46,18));
    p.end();

    QImage* bk_frame = new QImage(QSize(ui->lbBackground->width(),ui->lbBackground->height()), QImage::Format_ARGB32);
    p.begin(bk_frame);
    p.setPen(QPen(QColor("#20888888")));
    p.setBrush(QBrush(QColor("#20888888"), Qt::SolidPattern));
    p.drawRect(QRect(0,0,bk_frame->width()-1,bk_frame->height()-1));
    p.end();
    ui->lbBackground->setPixmap(QPixmap::fromImage(*bk_frame));



    //LOAD CONFIG BB.ini
    QSettings sett("bb.ini", QSettings::IniFormat);

    right_offset = sett.value("MAIN/RightOffset", 25).toInt();
    top_offset = sett.value("MAIN/TopOffset", 60).toInt();

    keyenable1[0] = ui->lbB1;
    keyenable1[1] = ui->lbB2;
    keyenable1[2] = ui->lbB3;
    keyenable1[3] = ui->lbB4;


    keyenable2[0] = ui->cbB1;
    keyenable2[1] = ui->cbB2;
    keyenable2[2] = ui->cbB3;
    keyenable2[3] = ui->cbB4;


    ms = SystemMouseHook::instance();
    ms->setConnected(true);
    connect(ms, SIGNAL(keyLPressed(int, int)), this, SLOT(keyLPressed(int, int)));
    connect(ms, SIGNAL(keyLReleased(int, int)), this, SLOT(keyLReleased(int, int)));
}

void Clicker::updateGroupState(int num,  bool state){

    if(num <GROUPSNUM)
    {
        keyenable2[num]->setChecked(state);
        keyenable2[num]->setEnabled (true);
        if(keyenable2[num]->isChecked()){
            keyenable1[num]->setPixmap(QPixmap::fromImage(*green_frame));
        } else {
            keyenable1[num]->setPixmap(QPixmap::fromImage(*red_frame));
        }
    }
}

void Clicker::set_visual_skill_state(int num, bool state, bool enable, bool groupstate){
    if(enable){
        if(groupstate){
            if(state) {
                //keylabel[num]->setStyleSheet(StyleSheetLabel[3]); // GREEN

            } else {
                //keylabel[num]->setStyleSheet(StyleSheetLabel[1]); // RED
            }
        } else {
            //keylabel[num]->setStyleSheet(StyleSheetLabel[5]); // WHITE
        }
    } else {
       //keylabel[num]->setStyleSheet(StyleSheetLabel[4]); // GRAY
    }
}

void Clicker::showParserStatus(int updatetime, L2Window* l2w, QImage clicker_bk){
    qDebug("Clicker::showParserStatus(int updatetime) %d", updatetime);
    static int ellipsed_time=0;

    this->setVisible(this->underMouse() || l2w->isActiveWindow || (((HWND)(this->winId())) == ::GetForegroundWindow()));

    if(!isVisible()) return;

    QTime updateTime;
    updateTime.start();

    QImage* findbar_btn = new QImage(QSize(51,23), QImage::Format_ARGB32);
    *findbar_btn = *green_frame;
    l2w->getStatusBtn(findbar_btn, bFindBarsIsPressed);
    if(!findbar_btn->isNull()) ui->lbStatus->setPixmap(QPixmap::fromImage(*findbar_btn));
    delete findbar_btn;

    QRect l2rect = l2w->getL2WRect();
    this->resize(l2rect.width(),l2rect.height());
    this->move(l2rect.x(),l2rect.y());
    ui->lbBackground->resize(l2rect.width(),l2rect.height());
    ui->lbBackground->move(0,0);

//    QImage* clicker_bk = new QImage(QSize(l2rect.width(),l2rect.height()), QImage::Format_ARGB32);
    if(!clicker_bk.isNull()){
//        l2w->getStatusBk(clicker_bk);
        ui->lbBackground->setPixmap(QPixmap::fromImage(clicker_bk));
//        delete clicker_bk;
    }

    QPoint topright;
    topright.setX(this->width()-ui->fr_controls->width() - right_offset);
    topright.setY(top_offset);

    ui->fr_controls->move(topright);

    ellipsed_time=((ellipsed_time*5)+updatetime)/6;
    //ellipsed_time=((ellipsed_time*5)+updateTime.elapsed())/6;

    QString label;
    QTextStream(&label) << ellipsed_time;
    ui->lcd_ellipsed_time->setDigitCount(5);
    if(ellipsed_time < 170){
        ui->lcd_ellipsed_time->setPalette(Qt::green);
    } else if(ellipsed_time < 200){
        ui->lcd_ellipsed_time->setPalette(Qt::yellow);
    } else {
        ui->lcd_ellipsed_time->setPalette(Qt::red);
    }
    ui->lcd_ellipsed_time->display(label);

}

// Broadcasts a key has been pressed
void Clicker::keyLPressed(int x, int y){
    qDebug("Clicker::keyLPressed()");

    if(isUnderWidget(ui->lbOnOff, x, y)) {
        emit set_operation_state(!ui->cbDongle->isChecked());
    } else if(isUnderWidget(ui->lbCtrl, x, y)) {
        emit set_ctrl(!ui->cbCtrl->isChecked());
    } else if(isUnderWidget(ui->lbShift, x, y)) {
        emit set_shift( !ui->cbShift->isChecked());
    } else if(isUnderWidget(ui->lbStatus, x, y)) {
        bFindBarsIsPressed = true;
        //if(!pressed_btn->isNull())ui->lbStatus->setPixmap(QPixmap::fromImage(*pressed_btn));
    } else if(isUnderWidget(ui->lcd_ellipsed_time, x, y)) {
        bSettingsIsPressed = true;
        QPalette palette;
        palette.setBrush(QPalette::Background,QBrush(QColor("#88FF0000"), Qt::SolidPattern));
        ui->lcd_ellipsed_time->setPalette(palette);
    } else {

        int i = 0;
        while( (i < GROUPSNUM) && (!isUnderWidget(keyenable1[i], x, y))){i++;}
        if(i<GROUPSNUM){            
            emit setGroupState(i, !keyenable2[i]->isChecked());
        }
    }



}

bool Clicker::isUnderWidget(QWidget* widget, int abs_x, int abs_y){
    QRect cb = widget->geometry();
    QPoint wdg = this->pos();
    if(abs_x < (wdg.x())                )     return false;
    if(abs_x > (wdg.x()+this->width())  )     return false;
    if(abs_y < (wdg.y())                )     return false;
    if(abs_y > (wdg.y()+this->height()) )     return false;
    int x=abs_x - wdg.x();
    int y=abs_y - wdg.y();
    wdg = ui->fr_controls->pos();
    qDebug("Clicker::keyLPressed() cb %d %d %d %d", cb.x(), cb.y(), cb.width(), cb.height());
    qDebug("Clicker::keyLPressed() wdg %d %d", wdg.x(), wdg.y());
    qDebug("Clicker::keyLPressed() clc %d %d", x, y);

    return (
                x > (wdg.x() + cb.x()) &&
                x < (wdg.x() + cb.x()+ cb.width()) &&
                y > (wdg.y() + cb.y()) &&
                y < (wdg.y() + cb.y()+ cb.height())

         );
}

// Broadcasts a key has been released
void Clicker::keyLReleased(int x, int y){
    qDebug("Clicker::keyLReleased()");

    if(isUnderWidget(ui->lbStatus, x, y) && bFindBarsIsPressed) {
           // Reset window
        emit resetBars();
    }
    if(bFindBarsIsPressed){
        bFindBarsIsPressed = false;
        //if(!released_btn->isNull())ui->lbStatus->setPixmap(QPixmap::fromImage(*released_btn));
    }
    if(isUnderWidget(ui->lcd_ellipsed_time, x, y)) {
        emit popupBbWindow();
    }
    if(bSettingsIsPressed){
            bSettingsIsPressed = false;
            QPalette palette;
            palette.setBrush(QPalette::Background,QBrush(QColor("#880000FF"), Qt::SolidPattern));
            ui->lcd_ellipsed_time->setPalette(palette);
    }
}

void Clicker::showDongleStatus(unsigned char d_stt,  int updatetime)
{
    //qDebug("Clicker::showDongleStatus");
    if(!isVisible()) return;

    ui->cbCtrl->setEnabled (false);

    Q_UNUSED(updatetime);


    ui->cbCtrl->setChecked((d_stt & (1 << DEVICE_CTRL)) > 0);
    ui->cbShift->setChecked((d_stt & (1 << DEVICE_SHIFT)) > 0);

    if((d_stt & (1<<DEVICE_STATUS)) == 0){
        ui->cbDongle->setChecked(false);
        ui->cbDongle->setEnabled(true);
   //     ui->cbDongle->setStyleSheet(StyleSheetCheckBox[1]); //RED
    } else if((d_stt & (1<<DEVICE_MODE)) == 0){
        ui->cbDongle->setChecked(true);
        ui->cbDongle->setEnabled(true);
   //     ui->cbDongle->setStyleSheet(StyleSheetCheckBox[3]); //GREEN
    } else {
        ui->cbDongle->setChecked(true);
        ui->cbDongle->setEnabled(true);
   //     ui->cbDongle->setStyleSheet(StyleSheetCheckBox[2]); //BLUE
    }


    if(ui->cbDongle->isChecked()){
        ui->lbOnOff->setPixmap(QPixmap::fromImage(*green_frame));
    } else {
        ui->lbOnOff->setPixmap(QPixmap::fromImage(*red_frame));
    }
    if(ui->cbShift->isChecked()){
        ui->lbShift->setPixmap(QPixmap::fromImage(*green_frame));
    } else {
        ui->lbShift->setPixmap(QPixmap::fromImage(*red_frame));
    }
    if(ui->cbCtrl->isChecked()){
        ui->lbCtrl->setPixmap(QPixmap::fromImage(*green_frame));
    } else {
        ui->lbCtrl->setPixmap(QPixmap::fromImage(*red_frame));
    }


}

Clicker::~Clicker()
{
    qDebug("Clicker::~Clicker()");
    delete ui;
}
