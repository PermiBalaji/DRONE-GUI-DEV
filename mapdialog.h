#ifndef MAPDIALOG_H
#define MAPDIALOG_H

#include <QDialog>
namespace Ui {
class MapDialog;
}

class MapDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MapDialog(QWidget *parent = nullptr);
    ~MapDialog();

private:
    Ui::MapDialog *ui;

};
#endif // MAPDIALOG_H
