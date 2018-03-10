#include "skillbar.h"

Skillbar::Skillbar(Box *parent) : Box(parent)
{
    caption = "Skill Box";
}



QImage* Skillbar::setSkillImg(int n, QImage* image){
    //qDebug("L2Window::findTool(int n) %d", n);  //23
    if(getState()) {
        skill[n].setSkillImg(image);
    }
    return skill[n].getSkillImg();
}

void Skillbar::initSkillbarFrame(){
    if(getState()) {
        for(int n=0;n<KEYNUM;n++){
            int k = n/12; //1
            //11
            int l = (n-k*12)/4; // 2
            int m = n - k*12 - l*4;  //3
            k = 4-k; // 3
            int sell_x = TOOLLOFFSET + (l*4 + m)*TOOLSELL + (l*4 + m)*TOOLHGAP + l*TOOLH2GAP;
            int sell_y = TOOLTOFFSET + k*TOOLSELL + k*TOOLVGAP;
            skill[n].setFrame(QRect(topleft.rx()+sell_x, topleft.ry()+sell_y, TOOLSELL, TOOLSELL));
        }
    }
}

void Skillbar::initSkillbarImage(QImage* image){
    if(getState()) {
        for(int n=0;n<KEYNUM;n++){
            skill[n].setSkillImg(image);
        }
    }
}

void Skillbar::checkSkill(int n, QImage* image){
    if(getState()) {
        //QImage t = parceSkillbar(n, image);
        //skill[n].checkSkill(&t);
        skill[n].checkSkill(image);
    }
}

void Skillbar::drawStatus(QImage* imgStatus, QRect r){
    QPainter p;
    p.begin(imgStatus);

    if(getLeftStatus()){
        p.setPen(QPen(QColor("#88442244")));
        p.setBrush(QBrush(QColor("#88442244"), Qt::SolidPattern));
    } else {
        p.setPen(QPen(QColor("#88FF0000")));
        p.setBrush(QBrush(QColor("#88FF0000"), Qt::SolidPattern));
    }
    p.drawRect(QRect(r.x(),r.y(),r.width(),r.height()));


    p.end();

}
