#include "window.h"
using namespace L2;
Window::Window(HWND winhwnd, QObject *parent) : QObject(parent)
{
    hwnd = winhwnd;
    image_width = 0;
    image_height = 0;
    L2icon = NULL;

    //LOAD CONFIG BB.ini
    QSettings sett("bb.ini", QSettings::IniFormat);


    bEnablePet = sett.value("MAIN/EnablePet", 0).toBool();
    bEnableParty = sett.value("MAIN/EnableParty", 0).toBool();
    bSearchTarget = sett.value("MAIN/SearchTarget", 0).toBool();
    bShowRange = sett.value("MAIN/ShowRange", 0).toBool();


    mainbarbox = new  Mainbox();
    mobbarbox  = new  Mobbox();
    petbarbox  = new  Petbox();
    partybox  = new  Partybox();
    skillbar = new Skillbar();
    resbox = new ResurrectionBox();
    targetbox = new TargetBox();

    resbox->setTimer(20);
    mobbarbox->setTimer(20);
    partybox->setTimer(20);
    skillbar->setTimer(20);


    mainbarbox->setSearchArea(QRect(0,0,100,100),50, 390);

    resetBars();
    skillsread = false;
}



void Window::capture(){
    //qDebug("L2Window::capture");

    if(IsIconic(hwnd) != 0){
        //qDebug("Window is iconic: %d", (int) hwnd);
        windowtopright.setX(0);
        windowtopright.setY(0);
        isActiveWindow = false;
        return ;
    }

    RECT windowRect;
    GetWindowRect(hwnd, &windowRect);

    HWND foreground = GetForegroundWindow();

    isActiveWindow = (foreground == hwnd);
    windowtopright.setX(windowRect.right);
    windowtopright.setY(windowRect.top);
    l2windowrect.setLeft(windowRect.left);
    l2windowrect.setRight(windowRect.right);
    l2windowrect.setTop(windowRect.top);
    l2windowrect.setBottom(windowRect.bottom);

    HDC hdcSrc = GetWindowDC(hwnd);
    if(hdcSrc == NULL){
        //qDebug("GetDC failed: %d", (int) hwnd);
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




void Window::check(){

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
        isActiveWindow = false;
        //err groupmanager->isL2Active(isActiveWindow);
        return;
    }


    bool invalidpet;
    bool invalidparty;
    bool invalidmain;
    bool invalidmob;

    capture();
    //err groupmanager->isL2Active(isActiveWindow);



    if(image.isNull()) {
        qWarning("image.isNull()");
        return;
    }



    if(status == L2_OFF || image_width != image.width() || image_height != image.height()){
        resetBars();
        image_width = image.width();
        image_height = image.height();
    }

    invalidmain = !mainbarbox->getState();
    invalidmob = !mobbarbox->getState();
    invalidpet = !petbarbox->getState();
    invalidparty = !partybox->getState();





    if(invalidmain)
        if(mainbarbox->findLeft(&image, "patterns\\main_left.bmp"))
            mainbarbox->findRight(&image, "patterns\\main_right.bmp");





    if(mainbarbox->getState()){
        if(invalidmain){
            mainbarbox->initBars();
            QImage icotmp=mainbarbox->copyIcon(&image);
            QPixmap pixmap(20,20);
            pixmap.convertFromImage(icotmp);
            if(L2icon) delete L2icon;
            L2icon = new QIcon(pixmap);
            mobbarbox->setSearchArea(QRect(image.width()/4,0,image.width()/2,image.height()),140, 410);

            if(bEnablePet){

                petbarbox->setSearchArea(QRect(300,image.height()*2/3,image.width()/3,image.height()/3),140, 410);
            }
            if(bEnableParty){
                partybox->setSearchArea(QRect(5,70,100,150),140, 410);
            }

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


        if(mobbarbox->getState()){
            if(invalidmob){
                mobbarbox->initBars();
                mobbarbox->findToken();
            }
        }


        if(bEnablePet){
            if((!petbarbox->getLeftStatus()) && (petbarbox->getTimer()))
                if(petbarbox->findLeft(&image,  "patterns\\pet_left.bmp"))
                    petbarbox->findRight(&image,  "patterns\\pet_right.bmp");

            if(petbarbox->getState()){
                if(invalidpet){
                    petbarbox->initBars();
                }
            }
            if((!petbarbox->getState()) )petbarbox->setTimer(200);
        }

        if(bEnableParty){
            if((!partybox->getLeftStatus()) && (partybox->getTimer()))
                if(partybox->findLeft(&image,  "patterns\\party2_left.bmp"))
                    partybox->findRight(&image,  "patterns\\party_right.bmp");

            if(partybox->getState()){
                if(invalidparty){
                    partybox->initBars();
                }
            }
            if((!partybox->getState()) ) partybox->setTimer(200);


        }
        if((!skillbar ->getState()) && (skillbar ->getTimer()))
            if(skillbar->findLeft(&image,  "patterns\\toolbar_left.bmp")){
                skillbar->initSkillbarFrame();
                if(!skillsread){
                    skillbar->initSkillbarImage(&image);
                    skillsread = true;
                }
            }
    }


    if(skillbar->getState() && skillbar->getTimer()) if(!skillbar->checkBoxExist(&image)){
        skillbar->reset();
        if(skillbar->findLeft(&image,  "patterns\\toolbar_left.bmp")) skillbar->initSkillbarFrame();
    }



    status = L2_ON;
    if(bEnablePet) {
        bPet = petbarbox->checkBoxExist(&image);
        if(bPet) petbarbox->checkPets(&image);
    }

    if(bEnableParty) {
        bParty = partybox->checkBoxExist(&image);
        //if(bParty)
            partybox->checkMembers(&image);
    }

    mainbarbox->checkBars(&image);

    static int lastHP = 0;
    lastHP = ((lastHP == 0)&&(getXP(idHP) > 100))? 0: getXP(idHP);

    if(lastHP < 1 )
    {
        if(resbox->getLeftStatus())
        {
            resbox->setOkDetected(resbox->checkBoxExist(&image));
        } else {
            if(resbox->getTimer())
            {
                resbox->setSearchArea(QRect(image.width()/2-133,image.height()/2-54,10,20),0, 0);
                resbox->findLeft(&image, "patterns\\res_left.bmp");
            }
        }
    } else {
        resbox->setOkDetected(false);
    }


    mobbarbox->checkTargetType(&image);


    mobbarbox->checkToken(&image);
    targetbox->reset();
    bInRange = false;

    if((mobbarbox->getTargetType() == TARGETMOB || mobbarbox->getTargetType() == TARGETCHAR)&& bSearchTarget)
    {
        targetbox->setSearchArea(targetbox->getRange(image.width(), image.height()),0, 0);
        if(!targetbox->findLeft(&image, "patterns\\b_pattern.bmp")) targetbox->findLeft(&image, "patterns\\r_pattern.bmp");
        bInRange = targetbox->getLeftStatus();
    }

    //if(skillbar->getState()) {
        //for(int n=0;n<KEYNUM;n++){
            //err if(getConditionSkill(n) && getConditionState(n)){
                //skillbar->checkSkill(n, &image);
            //}
        //}
    //}

    //for(int i = 0; i < KEYNUM; i++){  //48 keys
        //err groupmanager->set_l2_skill_state(i, isSkillConditionRdy(i));
    //}

    return;
}

void Window::resetBars(){
    mainbarbox->reset();
    mobbarbox->reset();
    petbarbox->reset();
    partybox->reset();
    skillbar->reset();
    resbox->reset();
    targetbox->reset();
    bPet = false;
    bParty = false;
    bInRange = false;
    status = L2_OFF;
}

QString Window::getTitle(){
    QString title;
    QTextStream st(&title);
    st << hwnd;
    return title;
}



int Window::getXP(int index){
    if(index < idMobHP) {
        return mainbarbox->getBoxXP(index);
    } else if(index < idPet1HP){
        return mobbarbox->getBoxXP(index);
    }else if(index < idPartyHP){
        if(bEnablePet) return petbarbox->getBoxXP(index);
        else return XP_ERR;
    }else {
        if(bEnableParty) return partybox->getBoxXP(index);
        else return XP_ERR;
    }
}

QRect Window::getBarRect(int index){
    if(index < idMobHP) {
        return mainbarbox->getBoxRect(index);
    } else if(index < idPet1HP){
        return mobbarbox->getBoxRect(index);
    }else if(index < idPartyHP){
        if(bEnablePet) return petbarbox->getBoxRect(index);
        else return QRect(0, 0, 0, 0);
    } else {
        if(bEnableParty) return partybox->getBoxRect(index);
        else return QRect(0, 0, 0, 0);
    }
}

 void Window::getStatusBtn(QImage* imgStatus, bool pressed){
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
    if(bEnableParty)  partybox->drawStatus(imgStatus, QRect(3,9,5,8));
    skillbar->drawStatus(imgStatus, QRect(22,12,20,5));
}

 void Window::drawOverlayedStatus(QPainter* p, QPen* skillpen){
    skillpen->setWidth(2);

    mobbarbox->drawOverlayedStatus(p, skillpen);
    mainbarbox->drawOverlayedStatus(p, skillpen);

    if(bEnableParty)
    {
        partybox->drawOverlayedStatus(p, skillpen);

    }

    static int lastHP = 0;
    lastHP = ((lastHP == 0)&&(getXP(idHP) > 100))? 0: getXP(idHP);

    if(lastHP < 1 ){
        //qDebug() << "Need to draw resurrection box";
        skillpen->setColor(QColor("#88888888"));
        skillpen->setWidth(1);
        p->drawRect(QRect(image.width()/2-133,image.height()/2-54,10,20));
        if(resbox->getLeftStatus())
        {
            //qDebug() << "draw resurrection box BoxExist: "<< resbox->getOkDetected();
            if(resbox->getOkDetected())
            {
                //qDebug() << "draw resurrection box (top, left) coord: "<< r.x() << ", " << r.y();
                //qDebug() << "draw resurrection box (w x h) dim: "<< r.width()<< " x " << r.height();
                if(resbox->isOkInFocus())
                {
                    skillpen->setColor(QColor("#8800FF00"));
                } else {
                    skillpen->setColor(QColor("#88FF0000"));
                }
                skillpen->setWidth(1);
                QRect r;
                r = resbox->getOkRect();
                p->drawRect(resbox->getOkRect());
                QPoint m = resbox->getMouseCoord();
                p->drawEllipse(QRect(m.x()-2, m.y()-2,5,5));
            }
        }
    }
    if(bShowRange && ((mobbarbox->getTargetType() == TARGETMOB || mobbarbox->getTargetType() == TARGETCHAR)))
    {
        if(targetbox->getLeftStatus())
        {
            skillpen->setColor(QColor("#8800FF00"));
            p->setPen(*skillpen);
            p->drawRect(targetbox->getRange(image.width(), image.height()));
        }
    }
 }

