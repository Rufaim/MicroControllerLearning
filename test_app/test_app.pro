include($$PWD/../common.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

win32 {
    CONFIG -= debug_and_release
}

SOURCES += main.cpp \
    programe_executer.cpp


LIBS += -L"$$OUT_PWD/../libs/controller" -l:libcontroller.a \
        -L"$$OUT_PWD/../libs/utils" -l:libutils.a \
        -pthread

HEADERS += \
    programe_executer.h

