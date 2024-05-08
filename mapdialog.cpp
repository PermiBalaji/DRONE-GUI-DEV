#include "mapdialog.h"
#include "ui_mapdialog.h"

#include<QQuickItem>
#include<QtWebEngineWidgets/QWebEngineView>
#include<QtWebEngineWidgets/QWebEnginePage>
#include<QtWebEngineWidgets/QWebEngineSettings>
#include<QWebEngineProfile>
MapDialog::MapDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MapDialog)
{
    ui->setupUi(this);
    QWebEngineView* webview=new QWebEngineView;
    QUrl url=QUrl("qrc:/Styles/map.html");
    webview->page()->load(url);

    ui->verticalLayout->addWidget(webview);

}

MapDialog::~MapDialog()
{
    delete ui;
}
