# This Makefile.am is in the public domain
AM_CPPFLAGS = \
  -I$(top_srcdir)/src/include \
  -I$(top_srcdir)/src/microspdy

AM_CFLAGS = $(HIDDEN_VISIBILITY_CFLAGS)


lib_LTLIBRARIES = \
  libmicrospdy.la

libmicrospdy_la_SOURCES = \
  io.h io.c \
  io_openssl.h io_openssl.c \
  io_raw.h io_raw.c \
  structures.h structures.c \
  internal.h internal.c \
  daemon.h daemon.c \
  stream.h stream.c \
  compression.h compression.c \
  session.h session.c \
  applicationlayer.c applicationlayer.h \
  alstructures.c alstructures.h 
libmicrospdy_la_LIBADD = \
  $(SPDY_LIBDEPS)

libmicrospdy_la_LDFLAGS = \
  $(SPDY_LIB_LDFLAGS)

libmicrospdy_la_CPPFLAGS = \
  $(AM_CPPFLAGS) $(SPDY_LIB_CPPFLAGS) \
  -DBUILDING_MHD_LIB=1

libmicrospdy_la_CFLAGS = -Wextra \
  $(AM_CFLAGS) $(SPDY_LIB_CFLAGS)


if USE_COVERAGE
  AM_CFLAGS += --coverage
endif
