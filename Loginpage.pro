QT       += core gui multimedia multimediawidgets location quickwidgets webengine webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Resources.cpp \
    loginpage.cpp \
    main.cpp \
    mainwindow.cpp \
    mapdialog.cpp \
    mapwindow.cpp

HEADERS += \
    loginpage.h \
    mainwindow.h \
    mapdialog.h \
    mapwindow.h

FORMS += \
    loginpage.ui \
    mainwindow.ui \
    mapdialog.ui \
    mapwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resource.qrc \
    qml.qrc \
    reso.qrc \
    resources.qrc \
    resources.qrc

DISTFILES += \
    map.qml \
    styles.css

