#ifndef FLY_H
#define FLY_H

#include <QDialog>

namespace Ui {
class Fly;
}

class Fly : public QDialog
{
    Q_OBJECT

public:
    explicit Fly(QWidget *parent = nullptr);
    ~Fly();

    QString getAction() const;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Fly *ui;
    QString action;
};

#endif // FLY_H
