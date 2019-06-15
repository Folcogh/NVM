#-------------------------------------------------
#
# Project created by QtCreator 2019-05-30T21:09:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NVM
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

CONFIG += c++14

SOURCES += \
        src/EditCinescenie.cpp \
        src/EditEvent.cpp \
        src/Event.cpp \
        src/ExecCinescenie.cpp \
        src/ExecEvent.cpp \
        src/MainWindow.cpp \
        src/main.cpp

HEADERS += \
        src/EditCinescenie.hpp \
        src/EditEvent.hpp \
        src/Event.hpp \
        src/ExecCinescenie.hpp \
        src/ExecEvent.hpp \
        src/MainWindow.hpp \
        src/Nvm.hpp

FORMS += \
        src/EditCinescenie.ui \
        src/EditEvent.ui \
        src/ExecCinescenie.ui \
        src/MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    TODO.txt
