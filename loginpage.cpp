#include "loginpage.h"
#include "ui_loginpage.h"
#include "mainwindow.h"
#include<QMessageBox>
LoginPage::LoginPage(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginPage)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()), this, SLOT(updateTime()));
    timer->start(1000);
    updateTime();

    //POWEROFF
    connect(ui->shutdown_button,SIGNAL(clicked(bool)),this,SLOT(close()));
    //RESTART
    connect(ui->restart_button,SIGNAL(clicked(bool)),this,SLOT(restart()));

    //Tabs
    ui->Registerform->hide();

}

LoginPage::~LoginPage()
{
    delete ui;
}

void LoginPage::updateTime()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();


    QString formattedDateTime = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");



    ui->date_time_label->setText(formattedDateTime);
}

void LoginPage::restart()
{
    close();
    LoginPage* newwindow= new LoginPage();
    newwindow->show();
    //deleteLater();
}
void LoginPage::on_Register_button_clicked()
{
    ui->loginForm->hide();
    ui->Registerform->show();

}
void LoginPage::on_Login_button_clicked()
{
    ui->Registerform->hide();
    ui->loginForm->show();
}
void LoginPage::on_loginbutton_clicked()
{
    droneui= new MainWindow(this);
    if(ui->username->text()=="AGFE" && ui->password->text()=="KGP"){
        hide();
        droneui->show();
    }
    else
    {
        QMessageBox::warning(this,"Invalid credentials","invalid username or password");
    }
}

