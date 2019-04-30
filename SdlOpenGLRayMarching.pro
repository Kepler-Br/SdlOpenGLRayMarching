TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    mainloop.cpp \
    customtime.cpp \
    camera.cpp \
    window.cpp \
    shader.cpp \
    inputmanager.cpp


LIBS += -lSDL2 -lGL -lGLEW

HEADERS += \
    mainloop.h \
    customtime.h \
    camera.h \
    window.h \
    shader.h \
    inputmanager.h

DISTFILES += \
    vert.glsl \
    frag.glsl

