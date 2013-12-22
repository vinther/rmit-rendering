#-------------------------------------------------
#
# Project created by QtCreator 2013-11-11T00:26:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = asset_packer_qt
TEMPLATE = app


SOURCES += main.cpp\
        assetpacker.cpp \
    filetreeitem.cpp \
    filetreemodel.cpp

HEADERS  += assetpacker.hpp \
    filetreeitem.hpp \
    filetreemodel.hpp

FORMS    += assetpacker.ui

QMAKE_CXXFLAGS += -std=c++11
INCLUDEPATH += ../../include
LIBS += -llzo2
