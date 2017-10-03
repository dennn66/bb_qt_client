#include "keysettingsdialog.h"


KeySettingsDialog::KeySettingsDialog(KeyCondition* con, L2Window* l2w, int tool, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeySettingsDialog)
{
    ui->setupUi(this);
    QDoubleValidator *vd = new QDoubleValidator();
    QIntValidator * vi = new QIntValidator();
    condition = con;
    QString value;
    QTextStream out(&value);
    currentl2w = l2w;
    toolNumber = tool;

    controlf[idCoolDown         ] = ui->leCoolDown;
    controlf[idPause            ] = ui->lePause;
    controlf[idCondition        ] = ui->leCondition;
    controlf[idSkillPause       ] = ui->leSkillPause;

    controli[idMinCP            ] = ui->leCPMin;
    controli[idMinHP            ] = ui->leHPMin;
    controli[idMinMP            ] = ui->leMPMin;
    controli[idMinVP            ] = ui->leVPMin;
    controli[idMinMobHP         ] = ui->leMobHPMin;
    controli[idMinMobMP         ] = ui->leMobMPMin;
    controli[idMinPet1HP        ] = ui->lePet1HPMin;
    controli[idMinPet2HP        ] = ui->lePet2HPMin;


    controli[idMaxCP            ] = ui->leCPMax;
    controli[idMaxHP            ] = ui->leHPMax;
    controli[idMaxMP            ] = ui->leMPMax;
    controli[idMaxVP            ] = ui->leVPMax;
    controli[idMaxMobHP         ] = ui->leMobHPMax;
    controli[idMaxMobMP         ] = ui->leMobMPMax;
    controli[idMaxPet1HP        ] = ui->lePet1HPMax;
    controli[idMaxPet2HP        ] = ui->lePet2HPMax;

    controlb[idCheckSkillTimeout] = ui->cbCheckSkill;
    controlb[idCheckPet         ] = ui->cbCheckPet;
    controlb[idPetState         ] = ui->cbPetState;
    controlb[idCtrl             ] = ui->cbCtrl;
    controlb[idShift            ] = ui->cbShift;


    for(int i = 0; i<KEY_DB_SIZE; i++){
        ui->cbKeyMnemonic->addItem(condition->getKeyMnemonicDB(i));
        if(strcmp(condition->getKeyMnemonicDB(i), condition->getKeyString().toStdString().c_str()) == 0){
            ui->cbKeyMnemonic->setCurrentIndex(i);
        }
    }



    controlb[idCheckSkillTimeout]->setChecked (condition->getConditionB(idCheckSkillTimeout));
    controlb[idCheckPet]->setChecked (condition->getConditionB(idCheckPet));
    controlb[idPetState]->setChecked (condition->getConditionB(idPetState));
    controlb[idPetState]->setEnabled(condition->getConditionB(idCheckPet));
    controlb[idCtrl]->setChecked (condition->getConditionB(idCtrl));
    controlb[idShift]->setChecked (condition->getConditionB(idShift));

    connect(ui->cbKeyMnemonic, SIGNAL(activated(int)), SLOT(cbKeyMnemonicActivated(int)));
    connect(ui->pbReload, SIGNAL(clicked()), SLOT(pbReloadClicked()));



    if(!currentl2w->getSkillImg(toolNumber)->isNull())    ui->tool_label->setPixmap(QPixmap::fromImage(*currentl2w->getSkillImg(toolNumber)));



    for(int i = idCoolDown; i<CONDFNUM; i++){
        value = "";
        controlf[i]->setValidator(vd);
        connect(controlf[i], SIGNAL(textChanged(QString)), SLOT(textFChanged(QString)));
        out << condition->getConditionF(i);
        controlf[i]->setText(value);
    }



    ui->leMPMin->setValidator(vi);
    for(int i = idMinCP; i <= idMaxPet2HP; i++){
        value = "";
        controli[i]->setValidator(vi);
        connect(controli[i], SIGNAL(textChanged(QString)), SLOT(textIChanged(QString)));
        if( condition->getConditionI(i)<=100){
            out << condition->getConditionI( i);
        }
        controli[i]->setText(value);
    }

    QGridLayout *layout_2 = new QGridLayout;
    QString key_label = "B";
    QTextStream key_label_stream(&key_label);

    for(int i=0;i<GROUPSNUM;i++)
    {
        int j=0;
        key_label = "B";
        key_label_stream <<  i+1;
        QGridLayout *sell  = new QGridLayout;
        controlb[idGroupB1+i] = new QCheckBox(key_label.toStdString().c_str());
        sell->addWidget(controlb[idGroupB1+i],0, 0);
        controlb[idGroupB1+i]->setChecked (condition->getGroupState(i));
        layout_2->addLayout(sell,i, j);
    }
    ui->chkbox_widget_2->setLayout(layout_2);

    QGridLayout *layout_3 = new QGridLayout;
    for(int i=0;i<TokensNum;i++)
    {
        int j=0;
        key_label = condition->conditionb_name[idCheckToken+i];
        QGridLayout *sell  = new QGridLayout;
        controlb[idCheckToken+i] = new QCheckBox(key_label.toStdString().c_str());
        sell->addWidget(controlb[idCheckToken+i],0, 0);
        controlb[idCheckToken+i]->setChecked (condition->getGroupState(i));
        controlb[idCheckToken+i]->setChecked (condition->getConditionB(idCheckToken+i));
        layout_3->addLayout(sell,i, j);

        j=1;
        key_label = condition->conditionb_name[idTokenCondition+i];
        QGridLayout *sell2  = new QGridLayout;
        controlb[idTokenCondition+i] = new QCheckBox(key_label.toStdString().c_str());
        sell2->addWidget(controlb[idTokenCondition+i],0, 0);
        controlb[idTokenCondition+i]->setChecked (condition->getConditionB(idTokenCondition+i));
        controlb[idTokenCondition+i]->setEnabled (condition->getConditionB(idCheckToken+i));
        layout_3->addLayout(sell2,i, j);

    }
    ui->chkbox_widget_3->setLayout(layout_3);




    lstAllSkills  = new QListWidget(this);

    lstAllSkills->resize( 4 + 32*12 + 6*(12-1) + 8, 2 + 32*3 + 5*(3 - 1) + 6);
    lstAllSkills->move(this->width() - lstAllSkills->width() - 5, this->height() - lstAllSkills->height() - 5);
    lstAllSkills->setIconSize( QSize( 32, 32 ) );
    lstAllSkills->setViewMode( QListWidget::IconMode );
    lstAllSkills->setSelectionMode(QAbstractItemView::SingleSelection);
    lstAllSkills->setDragEnabled(false);
    lstAllSkills->setDragDropMode(QAbstractItemView::NoDragDrop);
    lstAllSkills->viewport()->setAcceptDrops(false);
    lstAllSkills->setDropIndicatorShown(false);
    lstAllSkills->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    lstAllSkills->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    for(int i = 0; i < idNoUseSkillNum; i ++){
        listNoUseSkill[i] = new QListWidgetItem;
        QString label;
        QTextStream label_stream(&label);
        label_stream << i+1;
        if(!currentl2w->getSkillImg(toolNumber)->isNull()) listNoUseSkill[i]->setIcon( QPixmap::fromImage(*currentl2w->getSkillImg(i)) );
        if( condition->getConditionI(idPauseSkillNum) == i){
             listNoUseSkill[i]->setFlags(  Qt::ItemIsEnabled);
        }  else {
            listNoUseSkill[i]->setFlags( Qt::NoItemFlags);
        }
            lstAllSkills->addItem(listNoUseSkill[i]);
    }

    lstAllSkills->show();


    connect(lstAllSkills, SIGNAL(itemClicked(QListWidgetItem*)), SLOT(itemChanged(QListWidgetItem*)));



    QGridLayout *layout_4 = new QGridLayout;
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<1;j++)
        {
        //int j=0;
            key_label = condition->conditionb_name[idTargetMeOrPet+i+j*2];
            QGridLayout *sell  = new QGridLayout;
            controlb[idTargetMeOrPet+i+j*2] = new QCheckBox(key_label.toStdString().c_str());
            sell->addWidget(controlb[idTargetMeOrPet+i+j*2],0, 0);
            controlb[idTargetMeOrPet+i+j*2]->setChecked (condition->getConditionB(idTargetMeOrPet+i+j*2));
            layout_4->addLayout(sell,i, j);
        }
    }
    ui->gbTargetType->setLayout(layout_4);

    for(int i=0;i<CONDBNUM;i++){
        if((i != idCheckStar) && (i != idStarCondition)) connect(controlb[i], SIGNAL(clicked(bool)), SLOT(cbKeyEnableBxClicked(bool)));
    }
}

KeySettingsDialog::~KeySettingsDialog()
{
    delete ui;
}

void KeySettingsDialog::itemChanged(QListWidgetItem* item){
    int index = -1;
    for(index=0; index < idNoUseSkillNum; index++){
        if(listNoUseSkill[index] == item) break;
    }
    if(index != -1 && (index != toolNumber)) {

        if(listNoUseSkill[index]->flags() == Qt::ItemIsEnabled){
            listNoUseSkill[index]->setFlags( Qt::NoItemFlags);
            condition->setConditionI(idPauseSkillNum, 0xFF);
        }  else {
            if(condition->getConditionI(idPauseSkillNum) < idNoUseSkillNum) listNoUseSkill[condition->getConditionI(idPauseSkillNum)]->setFlags( Qt::NoItemFlags);
            listNoUseSkill[index]->setFlags(  Qt::ItemIsEnabled);
            condition->setConditionI(idPauseSkillNum, index);
        }
        //condition->setConditionB(idNoUseSkillState+index, (listNoUseSkill[index]->flags() == Qt::ItemIsEnabled));
    }

}

void KeySettingsDialog::cbKeyEnableBxClicked(bool checked){
    qDebug("KeySettingsDialog::cbKeyEnableBxClicked(bool checked): %d", checked);
    QCheckBox* cb = dynamic_cast<QCheckBox*>(QObject::sender());
    if( cb != NULL )
    {
        int i = 0;
        while( (i < CONDBNUM) && controlb[i] != cb){i++;}
        if(i<CONDBNUM){
            condition->setConditionB(i, checked);
            if((i>=idCheckToken)&&(i<idCheckToken+4))    controlb[i+TokensNum]->setEnabled (checked);

            controlb[idPetState]->setEnabled(condition->getConditionB(idCheckPet));
        }
    }
}

void KeySettingsDialog::textFChanged(QString text)
{
    QLineEdit* le = dynamic_cast<QLineEdit*>(QObject::sender());
    int i = idCoolDown;

    while( (i< CONDFNUM) && controlf[i] != le){i++;}
    if(i<CONDFNUM){
        condition->setConditionF(i, atof(text.toStdString().c_str()));
    }
}

void KeySettingsDialog::textIChanged(QString text)
{
    QLineEdit* le = dynamic_cast<QLineEdit*>(QObject::sender());
    int i = idMinCP;
    qDebug("%s",text.toStdString().c_str() );
    while( (i <= idMaxPet2HP) && controli[i] != le){i++;}
    if(i <= idMaxPet2HP){
        if(text == "") {
            condition->setConditionI( i, 0xFF);
        } else {
            condition->setConditionI( i, atoi(text.toStdString().c_str()));
            if( condition->getConditionI( i)>100){
                condition->setConditionI( i, 0xFF);
            }
        }
    }
}


void KeySettingsDialog::pbReloadClicked(){
    qDebug("void KeySettingsDialog::pbReloadClicked()");
    currentl2w->setSkillImg(toolNumber);
    if(!currentl2w->getSkillImg(toolNumber)->isNull())    ui->tool_label->setPixmap(QPixmap::fromImage(*currentl2w->getSkillImg(toolNumber)));
}

void KeySettingsDialog::cbKeyMnemonicActivated(int index)
{
    condition->selectKey(index);

}
