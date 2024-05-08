
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include<QTimer>
#include<vector>
#include<QtMultimedia>
#include<QtMultimediaWidgets>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MapDialog;
struct Point {
    double lat;
    double lon;
};
class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void myfunction();

private slots:

    void on_Set_button_clicked();

    void on_reset_button_clicked();

    void on_altitude_slider_valueChanged(int value);

    void on_Battery_progress_valueChanged(int value);

    void on_togglespraychanged(int state);

    void on_actionPlan_triggered();

    void on_pushButton_5_clicked();
    void readDistance();
    void requestData();
    void on_calculate_clicked();
    void on_GPS_values_clicked();

private:
    Ui::MainWindow *ui;
    QTimer *timer_mw;
    QMediaPlayer* player;
    QVideoWidget* video;
    MapDialog* mapdialog;
    QTcpSocket socket;
    QVector<Point> coordinates;
    Point referencePoint;
    bool buttonClicked = false;
    std::vector<int> xpoints;
    std::vector<int> ypoints;
    QVector<double> latitudes;
    QVector<double> longitudes;
    double deg2rad(double deg);
    double calculateDistance(const Point& p1, const Point& p2);
    double calculatePolygonAreaFromUTM(std::vector<int>& x, std::vector<int>& y);
    int convertDistanceToFluidLevel(int distance);
    double calculateApplicationRate(double area, int distance);

};
#endif // MAINWINDOW_H


