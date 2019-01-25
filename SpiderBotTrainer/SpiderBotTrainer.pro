#-------------------------------------------------
#
# Project created by QtCreator 2019-01-04T21:19:27
#
#-------------------------------------------------

QT       += core gui 3drender 3dextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SpiderBotTrainer
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
        spbmainwindow.cpp \
    sbmsettings.cpp \
    sbmviewer.cpp \
    sbmfootviewer.cpp \
    sbmfooteditor.cpp \
    node3dfootelement.cpp \
    node3dfoot.cpp \
    cmdpositionitem.cpp \
	qsectorplanemesh.cpp \
    sbmcommandgenerator.cpp \
    footitem.cpp \
    qsbmcameracontroller.cpp \
    sbmsteptime.cpp \
    projectsavecontroller.cpp \
    projectdata.cpp \
    projectdatacommand.cpp

HEADERS += \
        spbmainwindow.h \
    sbmsettings.h \
    sbmviewer.h \
    sbmfootviewer.h \
    sbmfooteditor.h \
    node3dfootelement.h \
    node3dfoot.h \
    testdata.h \
    cmdpositionitem.h \
	qsectorplanemesh.h \
    sbmcommandgenerator.h \
    footitem.h \
    qsbmcameracontroller.h \
    sbmsteptime.h \
    projectsavecontroller.h \
    projectdata.h \
    projectdatacommand.h

FORMS += \
        spbmainwindow.ui \
    sbmfooteditor.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
