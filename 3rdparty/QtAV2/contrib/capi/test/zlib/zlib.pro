QT = core
TEMPLATE = app
CONFIG -= app_bundle
SOURCES += \
    zlib_api.cpp \
    zlib_api_test.cpp
HEADERS += zlib_api.h

include(../../capi.pri)
