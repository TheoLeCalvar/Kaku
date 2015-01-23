#-------------------------------------------------
#
# Project created by QtCreator 2014-10-17T15:01:55
#
#-------------------------------------------------

QT += core gui
QT += widgets xml svg
QT += network

CONFIG += c++11

TARGET = KakuClient
TEMPLATE = app

OBJECTS_DIR = obj/
MOC_DIR = moc/

QMAKE_CXXFLAGS = -Iinclude/

TRANSLATIONS = kaku_en.ts

SOURCES += src/main.cpp\
    src/mainwindow.cpp \
    src/myqview.cpp \
    src/mychatbox.cpp \
    src/mydemandemain.cpp \
    src/kakuselectcolor.cpp \
    src/mySvg.cpp \
    src/kakuselectcolorborder.cpp \
    src/dpointpolygone.cpp \
    src/ClientSocket.cpp \
    src/dialogConnexion.cpp

HEADERS  += include/mainwindow.h \
    include/myqview.h \
    include/mychatbox.h \
    include/mydemandemain.h \
    include/kakuselectcolor.h \
    include/mySvg.h \
    include/kakuselectcolorborder.h \
    include/dpointpolygone.h \
    include/dialogConnexion.h \
    include/ClientSocket.h

DISTFILES += \
    kaku_en.ts

