#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QMainWindow>
#include<QDateTime>
#include<QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginPage; }
QT_END_NAMESPACE
class MainWindow;
class LoginPage : public QMainWindow
{
    Q_OBJECT

public:
    LoginPage(QWidget *parent = nullptr);
    ~LoginPage();
private slots:
    void updateTime();
    void restart();

    void on_Register_button_clicked();

    void on_Login_button_clicked();


    void on_loginbutton_clicked();

private:
    Ui::LoginPage *ui;
    QTimer* timer;
    MainWindow* droneui;

};
#endif // LOGINPAGE_H
