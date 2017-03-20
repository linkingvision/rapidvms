TEMPLATE = lib
TARGET = onvifc
DESTDIR = ./lib/
QT += core network xml script xmlpatterns widgets gui
CONFIG += help staticlib
DEFINES += QT_DLL QT_NETWORK_LIB QT_HELP_LIB QT_SCRIPT_LIB QT_WIDGETS_LIB QT_XML_LIB QT_XMLPATTERNS_LIB ONVIFC_LIB
INCLUDEPATH += ./GeneratedFiles/Debug \
    ./GeneratedFiles \
    . \
    $(QTDIR)/mkspecs/linux-g++ \
    ./GeneratedFiles/Debug \
    ./../include \
    ./../include/device_management \
    ./../include/media_management \
    ./../include/ptz_management
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(../prj_linux/onvifc.pri)
