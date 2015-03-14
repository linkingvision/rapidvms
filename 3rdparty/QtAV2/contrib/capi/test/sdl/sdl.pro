QT = core
TEMPLATE = app
CONFIG -= app_bundle
SOURCES += \
    sdl_api.cpp \
    sdl_api_test.cpp
HEADERS += sdl_api.h
INCLUDEPATH += /usr/local/include
include(../../capi.pri)

#need SDL_Main
mac: {
LIBS += -L/usr/local/lib -lSDLmain -Wl,-framework,Cocoa
LIBS += -lSDL
}
