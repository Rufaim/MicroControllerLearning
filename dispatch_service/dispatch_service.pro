include($$PWD/../common.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

win32 {
    CONFIG -= debug_and_release
}


LIBS += -L"$$OUT_PWD/../libs/controller" -l:libcontroller.a \
        -L"$$OUT_PWD/../libs/utils" -l:libutils.a \
        -lboost_filesystem \
        -lboost_system \
        -pthread

HEADERS += \
    programe_executer.h \
    programm_generator.h \
    task_executer.h

SOURCES += main.cpp \
    task_executer.cpp \
    programe_executer.cpp \
    programm_generator.cpp \

