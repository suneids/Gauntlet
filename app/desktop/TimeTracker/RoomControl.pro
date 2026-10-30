QT       += core gui serialport charts network
QT       += qml quick quickwidgets quick3d
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aggregatorclient.cpp \
    main.cpp \
    mainwindow.cpp \
    timetracker.cpp

HEADERS += \
    aggregatorclient.h \
    mainwindow.h \
    timetracker.h

FORMS += \
    timetracker.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
unix:!macx {
    LIBS += -lX11
}
RESOURCES += \
    resources.qrc

