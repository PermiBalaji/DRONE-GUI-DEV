#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QPixmap>
#include<QDateTime>
#include<QRegularExpression>
#include<QtMath>
#include "mapdialog.h"
//CODE FOR THE UTM
constexpr double a = 6378137.0;    // Semi-major axis
constexpr double f = 1 / 298.257223563;
constexpr double k0 = 0.9996;
constexpr double n = f / (2 - f);
constexpr double A = (a / (1 + n)) * (1 + (n * n )/ 4 + (n * n * n * n) / 64);
constexpr double alpha1 = (n / 2) - ((2 * n * n) / 3) + ((5 * n * n * n) / 16);
constexpr double alpha2 = (13 * n * n) / 48 - (3 * n * n * n) / 5;
constexpr double alpha3 = (61 * n * n * n) / 240;
void convertLatLonToUTM(double lat, double lon, double &northing, double &easting)
{
    double lambda0 = floor(lon / 6) * 6 + 3; // Calculate the central meridian for the UTM zone
    double N0 =0;      // False Northing

    double phi = lat * M_PI / 180.0; // Convert latitude to radians
    double lambda = lon * M_PI / 180.0; // Convert longitude to radians
    double t = sinh(atanh(sin(phi)) - ((2 * sqrt(n)) / (1 + n)) * atanh(((2 * sqrt(n)) / (1 + n)) * sin(phi)));
    double xi = atan(t / (cos(lambda - (lambda0 * M_PI / 180.0))));
    double eta = atanh(sin(lambda - (lambda0 * M_PI / 180.0)) / sqrt(1 + t * t));
    easting = 500000 + ((k0 * A) * (eta + (alpha1 * cos(2 * xi) * sinh(2 * eta))
                                    + (alpha2 * cos(4 * xi) * sinh(4 * eta)) + (alpha3 * cos(6 * xi) * sinh(6 * eta))));

    northing = N0 + ((k0 * A )* (xi + (alpha1 * sin(2 * xi) * cosh(2 * eta))
                                 + (alpha2 * sin(4 * xi) * cosh(4 * eta)) + (alpha3 * sin(6 * xi) * cosh(6 * eta))));
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
     //Timer
    setWindowFlags(Qt::Window);
    timer_mw=new QTimer(this);
    connect(timer_mw,SIGNAL(timeout()),this,SLOT(myfunction()));
    timer_mw->start(1000);
    ui->lineEdit_altitude->setPlaceholderText("Enter the altitude");
    ui->altitude_label->setText("0m");
    // Connect the slot to the valueChanged signal
    connect(ui->Battery_progress, SIGNAL(valueChanged(int)), this, SLOT(updateProgressBarColor(int)));
    ui->GPS_Values->setFixedSize(50,40);
   // ui->toolBar->addWidget(ui->emergencyStopButton);
    ui->toolBar->addWidget(ui->spray_label);

    //Video streaming
    player=new QMediaPlayer();
    video=new QVideoWidget();
    video->setGeometry(1,1,ui->Video_widget->width(),ui->Video_widget->height());
    video->setParent(ui->Video_widget);
    player->setVideoOutput(video);
    player->setMedia(QUrl("C:/Qtvideo/The Forest _ Cinematic Drone Footage.mp4"));
    video->setVisible(true);
    video->show();
    player->play();
    //Spray status
    connect(ui->checkBox_spray,&QCheckBox::stateChanged,this,&MainWindow::on_togglespraychanged);
    ui->CL_PB->setStyleSheet("QProgressBar::chunk { background-color:blue; }");
    //MAPS
    ui->CL_PB->setValue(0);
    mapdialog=new MapDialog(this);
    ui->lineEdit_area->setPlaceholderText("0.0");
    ui->lineEdit_application->setPlaceholderText("0.0");
    //COMMUNICATION PROTOCALS
    // Connect to the ESP32 server (replace with your ESP32 IP and port)
    socket.connectToHost("192.168.4.1", 80);

    // Connect the signal for reading data
    connect(&socket, &QTcpSocket::readyRead, this, &MainWindow::readDistance);

    // Create and start a timer to request data periodically
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::requestData);
    timer->start(5000); // Request data every 5 seconds

    // Connect the calculate button click signal to the slot
    connect(ui->calculate_button, &QPushButton::clicked, this, &MainWindow::on_calculate_clicked);

    // Connect the GPS_values button click signal to the slot
    connect(ui->GPS_Values, &QPushButton::clicked, this, &MainWindow::on_GPS_values_clicked);
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::myfunction()
{
    QTime time = QTime::currentTime();
    QString time_name= time.toString("hh : mm : ss");
    if((time.second()%2)==0){
        time_name[3]=' ';
        time_name[8]=' ';
    }
    ui->labeltime->setText(time_name);

}
void MainWindow::on_Set_button_clicked()
{
    bool ok=true;
    ui->altitude_label->setText(QString::number(ui->lineEdit_altitude->text().toInt(&ok) )+"m");
    ui->altitude_slider->setValue(ui->lineEdit_altitude->text().toInt(&ok));
}
void MainWindow::on_reset_button_clicked()
{
    ui->altitude_label->setText(QString::number(0)+"m");
    ui->altitude_slider->setValue(0);
    ui->lineEdit_altitude->setText(QString::number(0));
}
void MainWindow::on_altitude_slider_valueChanged(int value)
{
    ui->altitude_label->setText(QString::number(ui->altitude_slider->value()) + "m");
    ui->lineEdit_altitude->setText(QString::number(ui->altitude_slider->value()));
    ui->Battery_progress->setValue(ui->altitude_slider->value());
}
void MainWindow::on_Battery_progress_valueChanged(int value)
{
    if (value <= 20) {
        ui->Battery_progress->setStyleSheet("QProgressBar::chunk { background-color: red; }");
    } else if (value <= 60) {
        ui->Battery_progress->setStyleSheet("QProgressBar::chunk { background-color: yellow; }");
    } else {
        ui->Battery_progress->setStyleSheet("QProgressBar::chunk { background-color: green; }");
    }
}
//For Spray Status
void MainWindow::on_togglespraychanged(int state)
{

    if(state==Qt::Checked){
      ui->spray_label->setText("<b><font color='green'>Spraying Started..</font></b>");
    }
    else{
      ui->spray_label->setText("<b><font color='red'>Spraying Stopped..</font></b>");
    }
}
void MainWindow::on_actionPlan_triggered()
{
    mapdialog->show();
}
void MainWindow::on_pushButton_5_clicked()
{
    connect(ui->pushButton_5,SIGNAL(clicked(bool)),this,SLOT(close()));
}
double LD=0.0;
//READING THE DATA FROM THE ESP32 DEV BOARD
void MainWindow::readDistance()
{
    static QString partialLatitude;
    static QString partialLongitude;

    while (socket.canReadLine()) {
      QByteArray data = socket.readLine();
      if (!data.isEmpty()) {
          QString message = QString::fromUtf8(data).trimmed();
          qDebug() << "Received message:" << message;

          if (message.startsWith("Distance: ")) {
              int distance = message.mid(10).toInt();
              if (distance != 0) {
                  qDebug() << "Received Distance:" << distance;
                  LD=distance;
                  int fluidLevel = convertDistanceToFluidLevel(distance);
                  qDebug() << "Fluid Level:" << fluidLevel;
                  // Update the progress bar with the calculated fluid level
                  ui->CL_PB->setValue(fluidLevel);
              }
          } else if (message.startsWith("Latitude: ")) {
              partialLatitude = message.mid(10);
          } else if (message.startsWith("Longitude: ")) {
              partialLongitude = message.mid(11);

              if (!partialLatitude.isEmpty() && !partialLongitude.isEmpty() && buttonClicked) {
                  double latitude = partialLatitude.toDouble();
                  double longitude = partialLongitude.toDouble();

                  qDebug() << "Received Latitude:" << QString::number(latitude, 'f', 6);
                  qDebug() << "Received Longitude:" << QString::number(longitude, 'f', 6);
                  latitudes.push_back((QString::number(latitude, 'f', 6)).toDouble());
                  longitudes.push_back((QString::number(longitude, 'f', 6)).toDouble());
                  double northing, easting;
                  convertLatLonToUTM(latitude, longitude, northing, easting);
                  xpoints.push_back(static_cast<int>(easting));
                  ypoints.push_back(static_cast<int>(northing));

                  // Reset the partial latitude and longitude
                  partialLatitude.clear();
                  partialLongitude.clear();
                  buttonClicked = false;
              }
          } else {
              qDebug() << "Incomplete or invalid data received:" << message;
          }
      }
    }

    if (socket.state() == QAbstractSocket::ConnectedState) {
      qDebug() << "Socket is connected.";
    } else {
      qDebug() << "Socket is NOT connected.";
    }

    // Reset the button clicked flag after processing the coordinates

}

void MainWindow::requestData()
{
    qDebug() << "Requesting distance...";

    // Reconnect if the socket is not connected
    if (socket.state() != QAbstractSocket::ConnectedState) {
      qDebug() << "Socket is not connected. Reconnecting...";

      socket.connectToHost("192.168.4.1", 80);
      return;
    }

    // Send a request to the ESP32 to get the distance
    qint64 bytesWritten = socket.write("GET_DISTANCE\n");
    if (bytesWritten == -1) {
      qDebug() << "Error writing to socket:" << socket.errorString();
    } else {
      qDebug() << "Request sent successfully.";
    }
    socket.flush();
}

void MainWindow::on_calculate_clicked()
{
    if (xpoints.size() < 3 || ypoints.size() < 3) { // Need at least 3 points to form a polygon
      qDebug() << "Not enough coordinates to calculate area.";
      return;
    }

    // Add the first coordinate to the end to close the polygon
    xpoints.push_back(xpoints.front());
    ypoints.push_back(ypoints.front());
    //Lets print the lat/long values
    for(int i=0; i<latitudes.size(); i++){
      qDebug()<<QString::number(latitudes[i], 'f', 6)<<"  "<<QString::number(longitudes[i], 'f', 6);
    }
    //Lets print the UTM Coordinates
    for(int i=0; i<xpoints.size(); i++){
      qDebug()<<xpoints[i]<<"  "<<ypoints[i];
    }
    // Calculate the area
    double area = calculatePolygonAreaFromUTM(xpoints, ypoints);
    // Print or use the calculated area
    qDebug() << "Area: " << area <<"square meters";
    ui->lineEdit_area->setText(QString::number(area) + " m²");
    double AR=calculateApplicationRate(area,LD);
    AR=AR*10;
    ui->lineEdit_application->setText(QString::number(AR)+"L/ha");
    // Clear the vectors to prepare for new coordinates
    xpoints.clear();
    ypoints.clear();

}
std::vector<int> liquidDrained = {0, 0, 0, 1240, 1080, 1120, 960, 880, 840, 680, 460, 180, 200};

double MainWindow::calculateApplicationRate(double area, int distance)
{
    // Calculate the total liquid drained from 3 cm to the current distance
    int totalLiquidDrained = 0;
    if(distance>=12) return 8000;
    for (int i = 2; i <= distance; ++i) {
      totalLiquidDrained += liquidDrained[i];
    }
    // Calculate the application rate
    double applicationRate = totalLiquidDrained / area;
    return applicationRate;
}
void MainWindow::on_GPS_values_clicked()
{
    // Set the button clicked flag to true
    buttonClicked = true;
    // Print a message to indicate that GPS values are being taken
    qDebug() << "GPS values are being taken...";
}

double MainWindow::deg2rad(double deg)
{
    return deg * ((22/7) / 180.0);
}

double MainWindow::calculatePolygonAreaFromUTM(std::vector<int>& x, std::vector<int>& y)
{
    double area = 0.0;
    int j = x.size() - 1;
    for (int i = 0; i < x.size(); i++) {
      area = area + (x[j] + x[i]) * (y[j] - y[i]);
      j = i;
    }
    area = area / 2;
    if (area < 0)
      area = area * -1;
    return area;
}
int MainWindow::convertDistanceToFluidLevel(int distance)
{
    if (distance == 2) {
      return 100; // If the distance is 2 cm, set the fluid level to 100%
    } else if (distance == 3) {
      return 90; // If the distance is 3 cm, set the fluid level to 90%
    } else if (distance == 4) {
      return 80; // If the distance is 4 cm, set the fluid level to 80%
    } else if (distance == 5) {
      return 70; // If the distance is 5 cm, set the fluid level to 70%
    } else if (distance == 6) {
      return 60; // If the distance is 6 cm, set the fluid level to 60%
    } else if (distance == 7) {
      return 50; // If the distance is 7 cm, set the fluid level to 50%
    } else if (distance == 8) {
      return 40; // If the distance is 8 cm, set the fluid level to 40%
    } else if (distance == 9) {
      return 30; // If the distance is 9 cm, set the fluid level to 30%
    } else if (distance == 10) {
      return 20; // If the distance is 10 cm, set the fluid level to 20%
    } else if (distance == 11) {
      return 10; // If the distance is 11 cm, set the fluid level to 10%
    } else if (distance >= 12) {
      return 0; // If the distance is 12 cm or more, set the fluid level to 0%
    }
    return 0; // Default to 0 if the distance doesn't match any condition
}



