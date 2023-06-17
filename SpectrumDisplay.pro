#-------------------------------------------------
#
# Project created by QtCreator 2021-09-02T01:37:12
#
#-------------------------------------------------

QT       += core gui

QT += multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SpectrumDisplay
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    spectrum.cpp \
    FFT.cpp \
    backgroundwidget.cpp \
    audiorecordthread.cpp \
    commonhelp.cpp \
    colorselectwidget.cpp \
    SmoothCurveGenerator.cpp \
    config.cpp

HEADERS += \
        mainwindow.h \
    FFT.h \
    spectrum.h \
    backgroundwidget.h \
    audiorecordthread.h \
    commonhelp.h \
    fftw3.h \
    colorselectwidget.h \
    SmoothCurveGenerator.h \
    config.h

FORMS += \
        mainwindow.ui

RESOURCES += \
    resource.qrc

RC_ICONS = assets/ico/icon.ico

LIBS += $$PWD/libfftw3f-3.dll

win32 {
    LIBS += -luser32
    LIBS += -lole32
    LIBS += -lksuser
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
