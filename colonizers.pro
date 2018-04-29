TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/engine.cpp \
    src/gamefield.cpp \
    src/interface.cpp \
    src/main.cpp

HEADERS += \
    include/engine.h \
    include/gamefield.h \
    include/interface.h \
    include/interface.h

INCLUDEPATH = ./include/

QMAKE_CXXFLAGS += -std=c++1z \
