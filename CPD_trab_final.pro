QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

#QTPLUGIN += qgif qpng qmovie qbitmap

CONFIG += static
QT_STYLE_OVERRIDE=fusion

#CONFIG += console
#CONFIG -= app_bundle

#CONFIG -= import_plugins
#CONFIG += Qt6Core.dll
#CONFIG += Qt6Gui.dll
#CONFIG += Qt6Widgets.dll

#QMAKE_CXXFLAGS += -static
#QMAKE_LFLAGS_WINDOWS += -static

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    alertwindow.cpp \
    customqlabel.cpp \
    errordialog.cpp \
    loadingwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    methods.cpp \
    playermodel.cpp

HEADERS += \
    alertwindow.h \
    customqlabel.h \
    errordialog.h \
    loadingwindow.h \
    mainwindow.h \
    methods.h \
    playermodel.h

FORMS += \
    alertwindow.ui \
    errordialog.ui \
    loadingwindow.ui \
    mainwindow.ui

#TRANSLATIONS += \
#    CPD_trab_final_pt_BR.ts
#CONFIG += lrelease
#CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


RC_ICONS = media\SOFIFADB.ico
