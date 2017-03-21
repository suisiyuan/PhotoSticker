#-------------------------------------------------
#
# Project created by QtCreator 2016-12-07T11:30:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32 {
    QT += axcontainer
}

DEFINES += HAVE_CONFIG_H

TARGET = PhotoSticker
TEMPLATE = app


RC_ICONS = res/images/icon.ico


INCLUDEPATH += 3rd
DEPENDPATH  += 3rd


SOURCES += main.cpp\
        widget.cpp \
    3rd/bitstream.c \
    3rd/mask.c \
    3rd/mmask.c \
    3rd/mqrspec.c \
    3rd/qrencode.c \
    3rd/qrinput.c \
    3rd/qrspec.c \
    3rd/rscode.c \
    3rd/split.c

HEADERS  += widget.h \
    3rd/bitstream.h \
    3rd/config.h \
    3rd/mask.h \
    3rd/mmask.h \
    3rd/mqrspec.h \
    3rd/qrencode.h \
    3rd/qrencode_inner.h \
    3rd/qrinput.h \
    3rd/qrspec.h \
    3rd/rscode.h \
    3rd/split.h

FORMS    += widget.ui

RESOURCES += \
    res.qrc

DISTFILES +=
