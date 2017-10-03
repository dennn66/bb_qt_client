#include "l2parser.h"


L2Parser::L2Parser(HWND winhwnd, QObject *parent) : QObject(parent)
{
    hwnd = winhwnd;
    image_width = 0;
    image_height = 0;
    L2icon = NULL;


    //LOAD CONFIG BB.ini
    QSettings sett("bb.ini", QSettings::IniFormat);


    bEnablePet = sett.value("MAIN/EnablePet", 0).toBool();

    mainbarbox = new  Mainbox();
    mobbarbox  = new  Mobbox();
    petbarbox  = new  Petbox();
    skillbar = new Skillbar();

    mainbarbox->setSearchArea(QRect(0,0,20,50),50, 390);


    resetBars();
    skillsread = false;

}



void L2Parser::capture(){
    qDebug("L2Window::capture");

    if(IsIconic(hwnd) != 0){
        qDebug("Window is iconic: %d", (int) hwnd);
        windowtopright.setX(0);
        windowtopright.setY(0);
        isActiveL2Window = false;
        return ;
    }

    RECT windowRect;
    GetWindowRect(hwnd, &windowRect);

    HWND foreground = GetForegroundWindow();

    isActiveL2Window = (foreground == hwnd);
    windowtopright.setX(windowRect.right);
    windowtopright.setY(windowRect.top);

    HDC hdcSrc = GetWindowDC(hwnd);
    if(hdcSrc == NULL){
        qDebug("GetDC failed: %d", (int) hwnd);
        return ;
    }
    if(     (windowRect.left   !=    -32000) &&
            (windowRect.top    !=    -32000) &&
            (windowRect.left   <  4000) &&
            (windowRect.right  <  4000) &&
            (windowRect.top    <  4000) &&
            (windowRect.bottom <  4000) &&
            (windowRect.left   <  windowRect.right) &&
            (windowRect.top    <  windowRect.bottom)
        ){
        HDC hdcDest = CreateCompatibleDC(hdcSrc);
        int width = windowRect.right - windowRect.left;
        int height = windowRect.bottom - windowRect.top;
        HBITMAP hBitmap = CreateCompatibleBitmap(hdcSrc, width, height);
        HGDIOBJ hOld = SelectObject(hdcDest, hBitmap);
        BitBlt(hdcDest, 0, 0, width, height, hdcSrc, 0, 0, SRCCOPY);
        SelectObject(hdcDest, hOld);
        DeleteDC(hdcDest);
        ReleaseDC(hwnd, hdcSrc);
        QPixmap pixelMap = QtWin::fromHBITMAP(hBitmap);
        DeleteObject(hBitmap);
        image = pixelMap.toImage();
    } else {
        ReleaseDC(hwnd, hdcSrc);
    }
    return ;
}



int L2Parser::check(){

    qDebug("L2Parser::check()  Thread: %d", (int) QThread::currentThreadId());

    static bool iconic = false;
    static int iconiccounter = 0;


    if(iconic == true){
        if(!(IsIconic(hwnd) != 0)){
            if(iconiccounter++ > 50){
                iconic = false;
            }

        } else {
            iconiccounter = 0;
        }
    }  else {
       iconic = IsIconic(hwnd) != 0;
       iconiccounter = 0;
    }
    if(iconic) {
        windowtopright.setX(0);
        windowtopright.setY(0);
        isActiveL2Window = false;
        return status;
    }


    bool invalidpet;
    bool invalidmain;
    bool invalidmob;

    capture();



    if(image.isNull()) {
        qDebug("image.isNull()");
        return status;
    }



    if(status == L2_OFF || image_width != image.width() || image_height != image.height()){
        resetBars();
        image_width = image.width();
        image_height = image.height();
    }

    invalidmain = !mainbarbox->state();
    invalidmob = !mobbarbox->state();
    invalidpet = !petbarbox->state();





    if(invalidmain)
        if(mainbarbox->findLeft(&image, "patterns\\main_left.bmp"))
            mainbarbox->findRight(&image, "patterns\\main_right.bmp");





    if(mainbarbox->state()){
        if(invalidmain){
            mainbarbox->initBars();
            QImage icotmp=mainbarbox->copyIcon(&image);
            QPixmap pixmap(20,20);
            pixmap.convertFromImage(icotmp);
            if(L2icon) delete L2icon;
            L2icon = new QIcon(pixmap);
            mobbarbox->setSearchArea(QRect(image.width()/4,0,image.width()/2,image.height()),140, 410);
            mobbarbox->setTimer(20);
            if(bEnablePet){
                petbarbox->setTimer(200);
                petbarbox->setSearchArea(QRect(300,image.height()*2/3,image.width()/3,image.height()/3),140, 410);
            }

            skillbar->setTimer(20);
            skillbar->setSearchArea(QRect(image.width()/4,
                                          image.height()/2,
                                          image.width()-521-image.width()/4,
                                          image.height()-200-image.height()/2),0, 0);


        }


        if(invalidmob && mobbarbox->getTimer()){
            if(mobbarbox->findLeft(&image, "patterns\\mob_left_close.bmp")) {
                mobbarbox->findRight(&image, "patterns\\mob_right_close.bmp");
            } else {
                if(mobbarbox->findLeft(&image, "patterns\\mob_left_open.bmp"))
                    mobbarbox->findRight(&image, "patterns\\mob_right_open.bmp");
            }
        }


        if(mobbarbox->state()){
            if(invalidmob){
                mobbarbox->initBars();
                mobbarbox->findToken();
            }
        }


        if(bEnablePet){
            if((!petbarbox->getLeftStatus()) && (petbarbox->getTimer()))
                if(petbarbox->findLeft(&image,  "patterns\\pet_left.bmp"))
                    petbarbox->findRight(&image,  "patterns\\pet_right.bmp");

            if(petbarbox->state()){
                if(invalidpet){
                    petbarbox->initBars();
                }
            }
        }


        if((!skillbar ->state()) && (skillbar ->getTimer()))
            if(skillbar->findLeft(&image,  "patterns\\toolbar_left.bmp")){
                skillbar->initSkillbarFrame();
                if(!skillsread){
                    skillbar->initSkillbarImage(&image);
                    skillsread = true;
                }
            }
    }


    if(skillbar->state() && skillbar->getTimer()) if(!skillbar->checkBoxExist(&image)){
        skillbar->reset();
        skillbar->findLeft(&image,  "patterns\\toolbar_left.bmp");
    }



    status = L2_ON;
    if(bEnablePet) bPet = petbarbox->checkBoxExist(&image);
    if(bEnablePet) petbarbox->checkPets(&image);



    mainbarbox->checkBars(&image);


    mobbarbox->checkTargetType(&image);


    mobbarbox->checkToken(&image);



    if(skillbar->state()) {
        for(int n=0;n<KEYNUM;n++){
            if(1){//getConditionSkill(n) && getConditionState(n)){
                skillbar->checkSkill(n, &image);
            }
        }
    }


/*
    QImage pattern;
    pattern.load("patterns\\mob_left_close.bmp");

    for(int im_w = 0; im_w < image.width(); im_w++ ) {
        for(int im_h = 0; im_h < image.height(); im_h++ ) {
            //qDebug("Read pattern");
            bool res = false;
            for(int p_w = 0; p_w < pattern.width(); p_w++ ) {
                for(int p_h = 0; p_h < pattern.height(); p_h++ ) {
                    QRgb frame_pix = pattern.pixel(QPoint(p_w, p_h));
                    if(frame_pix == Qt::white){
                        res = true;
                    } else {
                        QRgb image_pix = image.pixel(QPoint(im_w+p_w, im_h+p_h));
                        res = (image_pix == frame_pix);
                    }
                    if(!res) break;
                }
                if(!res) break;
            }
                    //bool res = CompareColors(frame_pix,image_pix, 5, false);

        }
    }


    if(bWriteTiming)
        if(fStream && fStream->device()){
            *fStream << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz") << ";" << "Test image scaning" << ";" << QDateTime::currentDateTime().toMSecsSinceEpoch() << ";" << lasttime.msecsTo(QDateTime::currentDateTime())  << "\r\n";
            lasttime = QDateTime::currentDateTime();
        }

*/

/*
    if(bWriteTiming){
        testFile->close();
    }
*/

    return status;
}

void L2Parser::resetBars(){
    mainbarbox->reset();
    mobbarbox->reset();
    petbarbox->reset();
    skillbar->reset();
    bPet = false;
    status = L2_OFF;
}


int L2Parser::getXP(int index){
    if(index < idMobHP) {
        return mainbarbox->getBoxXP(index);
    } else if(index < idPet1HP){
        return mobbarbox->getBoxXP(index);
    }else {
        if(bEnablePet) return petbarbox->getBoxXP(index);
        else return XP_ERR;
    }
}


 void L2Parser::getStatusBtn(QImage* imgStatus, bool pressed){
    QPainter p;

    if(pressed){
         p.begin(imgStatus);
         p.setPen(QPen(QColor("#4400FF00")));
         p.setBrush(QBrush(QColor("#4400FF00"), Qt::SolidPattern));
         p.drawRect(QRect(3,3,44,16));
         p.end();
    }

    mainbarbox->drawStatus(imgStatus, QRect(3,3,10,5));
    mobbarbox->drawStatus(imgStatus, QRect(18,3,14,3));
    if(bEnablePet) petbarbox->drawStatus(imgStatus, QRect(12,12,6,6));
    skillbar->drawStatus(imgStatus, QRect(22,12,20,5));
/*
     for(int k=0; k<3;  k++){
        for(int i=0; i<4;  i++){
             for(int j=0; j<4; j++){
                 QColor pixelcolor;
                 if(!getCurrentSettings()->condition[j*12+k*4+i]->getState()){
                     pixelcolor = QColor("#88442244");
                 } else if(isSkillRdy(j*12+k*4+i)){
                     pixelcolor = QColor("#8800FF00");

                 } else {
                     pixelcolor = QColor("#88FF0000");
                 }
                 imgStatus->setPixelColor(k*7+i+23,(3-j)+13,pixelcolor);
             }
         }
     }
*/

}


 QString L2Parser::getTitle(){
     QString title;
     QTextStream st(&title);
     st << getHWND();
     return title;
 }
