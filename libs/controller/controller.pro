include($$PWD/../../common.pri)

TARGET = controller

TEMPLATE = lib
CONFIG += staticlib c++11
CONFIG -= app_bundle
CONFIG -= qt

win32 {
    CONFIG -= debug_and_release
}

SOURCES += controller.cpp \
            command_list.cpp

HEADERS += \
    controller.h \
    command_list.h

LIBS += -L"$$OUT_PWD/../libs/utils" -l:libutils.a

