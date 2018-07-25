include($$PWD/../../common.pri)

TARGET = utils

TEMPLATE = lib
CONFIG += staticlib c++11
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CFLAGS_RELEASE += -O3
QMAKE_CXXFLAGS_RELEASE += -O3

win32 {
    CONFIG -= debug_and_release
}

SOURCES += \
        utils.cpp

HEADERS += \
        utils.h
