# This Makefile.am is in the public domain
SUBDIRS  = .

AM_CFLAGS =

if USE_COVERAGE
  AM_CFLAGS += -fprofile-arcs -ftest-coverage
endif

AM_CPPFLAGS = \
 -I$(top_srcdir) \
 -I$(top_srcdir)/src/include \
 -I$(top_srcdir)/src/applicationlayer \
 -DDATA_DIR=\"$(top_srcdir)/src/datadir/\" \
 $(LIBCURL_CPPFLAGS)

if !HAVE_W32
PERF_GET_CONCURRENT=perf_get_concurrent
endif

bin_PROGRAMS = \
 microspdy2http

microspdy2http_SOURCES = \
 proxy.c 
microspdy2http_LDADD = \
  $(top_builddir)/src/microspdy/libmicrospdy.la \
 -lz \
 $(LIBCURL)
