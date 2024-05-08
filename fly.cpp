#include "fly.h"
#include "ui_fly.h"

Fly::Fly(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Fly)
{
    ui->setupUi(this);
}

Fly::~Fly()
{
    delete ui;
}

void Fly::on_pushButton_clicked()
{
    QString act=ui->comboBox->currentText();
    if(!act.isEmpty()){
        action=act;
    }
    accept();
}


void Fly::on_pushButton_2_clicked()
{
    reject();
}

QString Fly::getAction() const
{
    return action;
}

