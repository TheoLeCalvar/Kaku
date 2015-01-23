#-------------------------------------------------
#
# Project created by QtCreator 2014-10-17T15:01:55
#
#-------------------------------------------------

QT += core network widgets

CONFIG += c++11

TARGET = KakuServer
TEMPLATE = app

OBJECTS_DIR = obj/
MOC_DIR = moc/

QMAKE_CXXFLAGS = -Iinclude/ -g

SOURCES += src/main.cpp\
    src/Server.cpp\
    src/ServerSocket.cpp\
    src/ServerWindow.cpp

HEADERS  += include/Server.h \
    include/ServerSocket.h \
    include/ServerWindow.h

