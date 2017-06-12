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


INCLUDEPATH +=  3rd/libqrencode \
                3rd/libzint \
                3rd/libpng \
                3rd/zlib

DEPENDPATH  +=  3rd/libqrencode \
                3rd/libzint


SOURCES +=  main.cpp\
            widget.cpp \
            3rd/libqrencode/bitstream.c \
            3rd/libqrencode/mask.c \
            3rd/libqrencode/mmask.c \
            3rd/libqrencode/mqrspec.c \
            3rd/libqrencode/qrencode.c \
            3rd/libqrencode/qrinput.c \
            3rd/libqrencode/qrspec.c \
            3rd/libqrencode/rscode.c \
            3rd/libqrencode/split.c \
            3rd/libzint/2of5.c \
            3rd/libzint/auspost.c \
            3rd/libzint/aztec.c \
            3rd/libzint/bmp.c \
            3rd/libzint/codablock.c \
            3rd/libzint/code.c \
            3rd/libzint/code1.c \
            3rd/libzint/code16k.c \
            3rd/libzint/code49.c \
            3rd/libzint/code128.c \
            3rd/libzint/common.c \
            3rd/libzint/composite.c \
            3rd/libzint/dllversion.c \
            3rd/libzint/dmatrix.c \
            3rd/libzint/dotcode.c \
            3rd/libzint/eci.c \
            3rd/libzint/emf.c \
            3rd/libzint/gif.c \
            3rd/libzint/gridmtx.c \
            3rd/libzint/gs1.c \
            3rd/libzint/hanxin.c \
            3rd/libzint/imail.c \
            3rd/libzint/large.c \
            3rd/libzint/library.c \
            3rd/libzint/maxicode.c \
            3rd/libzint/medical.c \
            3rd/libzint/pcx.c \
            3rd/libzint/pdf417.c \
            3rd/libzint/plessey.c \
            3rd/libzint/png.c \
            3rd/libzint/postal.c \
            3rd/libzint/ps.c \
            3rd/libzint/qr.c \
            3rd/libzint/raster.c \
            3rd/libzint/reedsol.c \
            3rd/libzint/render.c \
            3rd/libzint/rss.c \
            3rd/libzint/svg.c \
            3rd/libzint/telepen.c \
            3rd/libzint/tif.c \
            3rd/libzint/upcean.c

HEADERS +=  widget.h \
            app.h \
            3rd/libqrencode/bitstream.h \
            3rd/libqrencode/config.h \
            3rd/libqrencode/mask.h \
            3rd/libqrencode/mmask.h \
            3rd/libqrencode/mqrspec.h \
            3rd/libqrencode/qrencode.h \
            3rd/libqrencode/qrencode_inner.h \
            3rd/libqrencode/qrinput.h \
            3rd/libqrencode/qrspec.h \
            3rd/libqrencode/rscode.h \
            3rd/libqrencode/split.h \ 
            3rd/libzint/aztec.h \
            3rd/libzint/bmp.h \
            3rd/libzint/code1.h \
            3rd/libzint/code49.h \
            3rd/libzint/common.h \
            3rd/libzint/composite.h \
            3rd/libzint/dmatrix.h \
            3rd/libzint/eci.h \
            3rd/libzint/emf.h \
            3rd/libzint/font.h \
            3rd/libzint/gb2312.h \
            3rd/libzint/gb18030.h \
            3rd/libzint/gridmtx.h \
            3rd/libzint/gs1.h \
            3rd/libzint/hanxin.h \
            3rd/libzint/large.h \
            3rd/libzint/maxicode.h \
            3rd/libzint/ms_stdint.h \
            3rd/libzint/pcx.h \
            3rd/libzint/pdf417.h \
            3rd/libzint/qr.h \
            3rd/libzint/reedsol.h \
            3rd/libzint/rss.h \
            3rd/libzint/sjis.h \
            3rd/libzint/stdint_msvc.h \
            3rd/libzint/tif.h \
            3rd/libzint/zint.h

LIBS += \
        $$PWD/3rd/libpng/libpng16.lib \
        $$PWD/3rd/zlib/zlib.lib


FORMS    += widget.ui

RESOURCES += res.qrc
