include($$PWD/../common.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

win32 {
    CONFIG -= debug_and_release
}

INCLUDEPATH += ../dispatch_service/ \


SOURCES += main.cpp \
    ../dispatch_service/programe_executer.cpp \
    ../dispatch_service/task_executer.cpp \


LIBS += -L"$$OUT_PWD/../libs/controller" -l:libcontroller.a \
        -L"$$OUT_PWD/../libs/utils" -l:libutils.a \
        -pthread

HEADERS += \
    ../dispatch_service/programe_executer.h \
    ../dispatch_service/task_executer.h \


