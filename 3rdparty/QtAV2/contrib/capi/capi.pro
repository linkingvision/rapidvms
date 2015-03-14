TEMPLATE = subdirs
include(capi.pri)
CONFIG += build_tests
build_tests: {
SUBDIRS = test/zlib
have_sdl: SUBDIRS += test/sdl
}

OTHER_FILES += README.md
